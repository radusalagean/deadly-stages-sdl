#include "GameEntity.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

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

void GameEntity::update()
{
    position += velocity;
    positionPlusCenter = position + center;
}

void GameEntity::draw(Camera& camera)
{
    dstRect.x = GSCALE(position.getX()) - camera.position.getX();
    dstRect.y = GSCALE(position.getY()) - camera.position.getY();
    dstRect.w = GSCALE(width);
    dstRect.h = GSCALE(height);
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
    debugRect.x = GSCALE(position.getX()) + GSCALE(collisionRect.x) - camera.position.getX();
    debugRect.y = GSCALE(position.getY()) + GSCALE(collisionRect.y) - camera.position.getY();
    debugRect.w = GSCALE(collisionRect.w);
    debugRect.h = GSCALE(collisionRect.h);
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, debugRect, {0, 0, 255, 255}, 2);
}
#endif