#ifndef __SRC_GAMEENTITY_ENEMY_HPP__
#define __SRC_GAMEENTITY_ENEMY_HPP__

#include "GameEntity.hpp"
#include "../Level/Level.hpp"

class Enemy : public GameEntity
{
public:
    Enemy();
    ~Enemy();

    void init(Level& level, Vector2D& spawnPoint);

    // Speed
    float topSpeedPxPerSecond = 0.0f;
    float speedPxPerSecond = 0.0f;

    // Damage
    void receiveDamage(const int amount);
    void crush();

    Vector2D* target = nullptr;

    void update(Level& level);
    void draw(Camera& camera);

};

#endif // __SRC_GAMEENTITY_ENEMY_HPP__
