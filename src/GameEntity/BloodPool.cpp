#include "BloodPool.hpp"
#include "../Core/SDLUtils.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <algorithm>
#include "../Core/SDLUtils.hpp"

BloodPool::BloodPool(const Vector2D& position, SDL_Texture* texture) : 
    GameEntity()
{
    this->texture = texture;
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    setSize(width, height);
    setPosition(position - center);
}

BloodPool::~BloodPool()
{
    texture = nullptr;
}
