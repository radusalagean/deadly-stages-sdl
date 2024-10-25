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
            logd("Unknown projectile type: %d", projectileType);
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
    this->velocity = Vector2D(cos((rotation - 90) * M_PI / 180.0f), sin((rotation - 90) * M_PI / 180.0f)) * speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    GameEntity* firstCollidedEntity = nullptr;
    CollisionManager::processMovement(*this, velocity, level, &firstCollidedEntity);
    if (firstCollidedEntity != nullptr)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(firstCollidedEntity);
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
