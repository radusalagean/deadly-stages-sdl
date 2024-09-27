#include "GameEntity.hpp"

#include "../Game.hpp"

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
}

void GameEntity::draw(Camera& camera)
{
    dstRect.x = position.getX() - camera.position.getX();
    dstRect.y = position.getY() - camera.position.getY();
    dstRect.w = getScaledWidth();
    dstRect.h = getScaledHeight();
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}

Vector2D* GameEntity::getPosition()
{
    return &position;
}

void GameEntity::setPosition(Vector2D position)
{
    this->position = position;
}