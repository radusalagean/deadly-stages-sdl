#include "Bullet.hpp"

#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"
#include "Enemy.hpp"

Bullet::Bullet(Vector2D weaponPosition, float rotation, TexturePool& texturePool) : GameEntity()
{
    this->setSize(8, 16);
    this->setPosition(weaponPosition - this->center);
    this->setRotation(rotation);
    this->texturePath = "res/game_entity/bullet.png";
    this->load(texturePool);
    this->damageAmount = 1;
}

Bullet::~Bullet()
{

}

void Bullet::update(Level& level)
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

void Bullet::draw(Camera& camera)
{
    GameEntity::draw(camera);
}
