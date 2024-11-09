#include "BloodPool.hpp"
#include "../Core/SDLUtils.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <algorithm>
#include "../Core/SDLUtils.hpp"

BloodPool::BloodPool(const Vector2D& position, SDL_Texture* texture, int size) : 
    GameEntity()
{
    canCollide = false;
    this->texture = texture;
    setSize(size, size);
    setPosition(position - center);
}

BloodPool::~BloodPool()
{
    texture = nullptr;
}
