#include "GameEntity.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../Core/CollisionManager.hpp"
#include "../Level/Level.hpp"

GameEntity::GameEntity()
{

}

GameEntity::~GameEntity()
{
}

void GameEntity::load(TexturePool& texturePool)
{
    texturePool.load(texturePath);
    texture = texturePool.get(texturePath);
}

void GameEntity::update(Level& level)
{
    position += velocity;
    positionPlusCenter = position + center;
}

void GameEntity::draw(Camera& camera)
{
    if (dstRectScale == 1.0f)
    {
        dstRect.w = GSCALE(width);
        dstRect.h = GSCALE(height);
        dstRect.x = GSCALE(position.x) - camera.position.x;
        dstRect.y = GSCALE(position.y) - camera.position.y;
    }
    else
    {
        dstRect.w = GSCALE(width * dstRectScale);
        dstRect.h = GSCALE(height * dstRectScale);
        dstRect.x = GSCALE(positionPlusCenter.x - (dstRect.w / 2)) - camera.position.x;
        dstRect.y = GSCALE(positionPlusCenter.y - (dstRect.h / 2)) - camera.position.y;
    }
    
    SDL_RenderCopyEx(Game::renderer, texture, NULL, &dstRect, rotation, NULL, SDL_FLIP_NONE);
}

void GameEntity::setPosition(Vector2D position)
{
    this->position = position;
}

#ifdef DEBUG_DRAW_COLLISION_RECTS
void GameEntity::drawCollisionRect(Camera& camera) // Draw collision rect with blue
{
    SDL_Rect debugRect;
    debugRect.x = GSCALE(position.x) + GSCALE(collisionRect.x) - camera.position.x;
    debugRect.y = GSCALE(position.y) + GSCALE(collisionRect.y) - camera.position.y;
    debugRect.w = GSCALE(collisionRect.w);
    debugRect.h = GSCALE(collisionRect.h);
    Game::primitiveShapeHelper.drawRectOutline(debugRect, {0, 0, 255, 255}, 2);
}
#endif

void GameEntity::sendDamage(GameEntity* targetEntity)
{
    targetEntity->receiveDamage(damageAmount);
}

void GameEntity::receiveDamage(const int amount)
{
    if (currentHealth <= 0)
        return;
    currentHealth = std::max(0, currentHealth - amount);
    if (currentHealth == 0)
        pendingRemoval = true;
}
