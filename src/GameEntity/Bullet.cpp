#include "Bullet.hpp"

#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"

Bullet::Bullet(Vector2D weaponPosition, float rotation, TexturePool& texturePool)
{
    this->setSize(8, 16);
    this->position = weaponPosition - this->center;
    this->rotation = rotation;
    this->texturePath = "res/game_entity/bullet.png";
    this->collisionRect = {0, 0, this->width, this->height};
    this->load(texturePool);
}

Bullet::~Bullet()
{

}

void Bullet::update(Camera& camera)
{
    this->velocity = Vector2D(cos((rotation - 90) * M_PI / 180.0f), sin((rotation - 90) * M_PI / 180.0f)) * speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    GameEntity::update();

}

void Bullet::draw(Camera& camera)
{
    GameEntity::draw(camera);
}