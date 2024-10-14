#ifndef __SRC_GAMEENTITY_BLOODPOOL_HPP__
#define __SRC_GAMEENTITY_BLOODPOOL_HPP__

#include "GameEntity.hpp"
#include <vector>
#include <random>

class BloodPool : public GameEntity
{
public:
    BloodPool(const Vector2D& position);
    ~BloodPool();

private:
    static const int MIN_BLOOD_POOL_SIZE = 32;
    static const int MAX_BLOOD_POOL_SIZE = 48;
    int radius = 0;
    std::random_device rd;
    std::mt19937 gen;
    void generateBloodTexture();
};

#endif // __SRC_GAMEENTITY_BLOODPOOL_HPP__
