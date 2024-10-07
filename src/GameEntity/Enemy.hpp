#ifndef __SRC_GAMEENTITY_ENEMY_HPP__
#define __SRC_GAMEENTITY_ENEMY_HPP__

#include "GameEntity.hpp"
#include "../Level/Level.hpp"

class Enemy : public GameEntity
{
public:
    Enemy();
    ~Enemy();

    float speedPxPerSecond = 100.0f;

    Vector2D* target = nullptr;

    void update(Camera& camera, Level& level);
    void draw(Camera& camera);
};

#endif // __SRC_GAMEENTITY_ENEMY_HPP__
