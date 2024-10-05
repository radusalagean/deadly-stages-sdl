#include "GameEntity.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"

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