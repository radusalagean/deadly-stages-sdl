#include "Enemy.hpp"

#include <math.h>
#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"
#include <chrono>
#include "../Level/Level.hpp"
#include "../BloodParticle/BloodParticleManager.hpp"
#include "../GameEntity/Player.hpp"

Enemy::Enemy() : GameEntity()
{
    damageAmount = 1;
    maxHealth = 5;
    currentHealth = maxHealth;
    castShadow = true;
}

Enemy::~Enemy()
{
    
}

void Enemy::init(Level& level, Vector2D& spawnPoint)
{
    setPosition(spawnPoint);
    setSize(ENEMY_SIZE, ENEMY_SIZE);
    texturePath = "res/level/" + level.id + "/enemy.png";
    target = &level.player->positionPlusCenter;
    speedPxPerSecond = level.currentEnemySpeed;
    topSpeedPxPerSecond = level.currentEnemySpeed;
    bounty = ceil(topSpeedPxPerSecond);
}

void Enemy::update(Level& level)
{
    if (target != nullptr)
    {
        rotateToTarget(*target);
    }
    velocity.x = cos(rotation * M_PI / 180.0f);
    velocity.y = sin(rotation * M_PI / 180.0f);
    velocity = velocity * -speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    GameEntity* firstCollidedEntity = nullptr;
    CollisionManager::EntityType firstCollisionEntityType = CollisionManager::EntityType::NONE;
    CollisionManager::processMovement(*this, CollisionManager::EntityType::ENEMY, velocity, level, 
        &firstCollidedEntity, firstCollisionEntityType);
    if (firstCollidedEntity != nullptr && firstCollisionEntityType == CollisionManager::EntityType::PLAYER)
    {
        handleContactWithPlayer(level.player, level);
    }
    else
    {
        isInContactWithPlayer = false;
    }
    GameEntity::update(level);
}

void Enemy::draw(Camera& camera)
{
    GameEntity::draw(camera);
    
    #ifdef DEBUG_DRAW_COLLISION_RECTS
    drawCollisionRect(camera);
    #endif
}

void Enemy::handleContactWithPlayer(Player* player, Level& level)
{
    if (!isInContactWithPlayer) {
        isInContactWithPlayer = true;
        contactWithPlayerStartTime = std::chrono::steady_clock::now();
    } else {
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - contactWithPlayerStartTime).count();
        if (duration >= hitAfterContactDelayMs) {
            player->receiveDamage(damageAmount, level);
        }
    }
}

void Enemy::crush(Level& level)
{
    pendingRemoval = true;
    level.bloodParticleManager.createParticles(positionPlusCenter, 100);
    level.createBloodPool(positionPlusCenter);
}

void Enemy::receiveDamage(const int amount, Level& level)
{
    int speedDecreasePerHealthUnit = topSpeedPxPerSecond / maxHealth;
    speedPxPerSecond = topSpeedPxPerSecond - (speedDecreasePerHealthUnit * (maxHealth - currentHealth));
    GameEntity::receiveDamage(amount, level);
}
