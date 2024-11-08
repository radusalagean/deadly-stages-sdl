#include "Projectile.hpp"

#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"
#include "Enemy.hpp"

Projectile::Projectile(Vector2D weaponPosition, ProjectileType projectileType, int damage, float rotation, TexturePool& texturePool) : GameEntity()
{
    this->setPosition(weaponPosition - this->center);
    this->setRotation(rotation);
    switch (projectileType)
    {
        case ProjectileType::PROJECTILE_TYPE_9MM_BULLET:
            this->setSize(8, 16);
            this->texturePath = "res/game_entity/bullet.png";
            break;

        case ProjectileType::PROJECTILE_TYPE_00_BUCK_SHELL_PELLET:
            this->setSize(8, 8);
            this->texturePath = "res/game_entity/pellet.png";
            break;

        default:
            printf("Unknown projectile type: %d\n", projectileType);
            break;
    }
    this->load(texturePool);
    this->damageAmount = damage;
}

Projectile::~Projectile()
{

}

void Projectile::update(Level& level)
{
    this->velocity.x = cos((rotation - 90) * M_PI / 180.0f);
    this->velocity.y = sin((rotation - 90) * M_PI / 180.0f);
    this->velocity = this->velocity * speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    GameEntity* firstCollidedEntity = nullptr;
    CollisionManager::EntityType firstCollisionEntityType = CollisionManager::EntityType::NONE;
    CollisionManager::processMovement(*this, CollisionManager::EntityType::PROJECTILE, velocity, level, 
        &firstCollidedEntity, firstCollisionEntityType, false);
    if (firstCollidedEntity != nullptr && firstCollisionEntityType == CollisionManager::EntityType::ENEMY)
    {
        Enemy* enemy = static_cast<Enemy*>(firstCollidedEntity);
        if (enemy != nullptr)
        {
            enemy->receiveDamage(damageAmount, level);
        }
        pendingRemoval = true;
    }
    GameEntity::update(level);
}

void Projectile::draw(Camera& camera)
{
    GameEntity::draw(camera);
}
