#ifndef __SRC_GAMEENTITY_PLAYER_HPP__
#define __SRC_GAMEENTITY_PLAYER_HPP__

#include "GameEntity.hpp"

class Player : public GameEntity
{
public:
    Player();
    ~Player();

    const float speedPxPerSecond = 100.0f;
    const int maxHealth = 14;
    int currentHealth = maxHealth;

    void update(Camera& camera);
    void draw(Camera& camera);
};

#endif // __SRC_GAMEENTITY_PLAYER_HPP__
