#ifndef __SRC_GAMEENTITY_BLOODPOOL_HPP__
#define __SRC_GAMEENTITY_BLOODPOOL_HPP__

#include "GameEntity.hpp"
#include <vector>
#include <random>

class BloodPool : public GameEntity
{
public:
    BloodPool(const Vector2D& position, SDL_Texture* texture);
    ~BloodPool();
};

#endif // __SRC_GAMEENTITY_BLOODPOOL_HPP__
