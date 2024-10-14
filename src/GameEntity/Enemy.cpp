#include "Enemy.hpp"

#include <math.h>
#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"
#include <chrono>
#include "../Level/Level.hpp"
#include "../GameEntity/BloodParticleManager.hpp"

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
    setSize(32, 32);
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
        float angle = atan2(target->y - positionPlusCenter.y, target->x - positionPlusCenter.x);
        angle = angle * (180 / M_PI) - 180;
        setRotation(angle);
    }
    velocity = Vector2D(cos(rotation * M_PI / 180.0f), sin(rotation * M_PI / 180.0f)) * -speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    GameEntity* firstCollidedEntity = nullptr;
    CollisionManager::processMovement(*this, velocity, level, &firstCollidedEntity);
    Player* collidedPlayer = dynamic_cast<Player*>(firstCollidedEntity);
    if (collidedPlayer != nullptr)
    {
        handleContactWithPlayer(collidedPlayer, level);
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
