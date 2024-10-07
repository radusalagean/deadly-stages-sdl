#include "Enemy.hpp"

#include <math.h>
#include "../Game.hpp"
#include "../Core/CollisionManager.hpp"

Enemy::Enemy() : GameEntity()
{

}

Enemy::~Enemy()
{
    
}

void Enemy::update(Camera& camera, Level& level)
{
    if (target != nullptr)
    {
        float angle = atan2(target->getY() - positionPlusCenter.getY(), target->getX() - positionPlusCenter.getX());
        angle = angle * (180 / M_PI);
        rotation = angle - 180;
    }
    velocity = Vector2D(cos(rotation * M_PI / 180.0f), sin(rotation * M_PI / 180.0f)) * -speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
    CollisionManager::processMovement(*this, velocity, level);
    GameEntity::update();
}

void Enemy::draw(Camera& camera)
{
    GameEntity::draw(camera);
    
    #ifdef DEBUG_DRAW_COLLISION_RECTS
    drawCollisionRect(camera);
    #endif
}
