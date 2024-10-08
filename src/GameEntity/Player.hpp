#ifndef __SRC_GAMEENTITY_PLAYER_HPP__
#define __SRC_GAMEENTITY_PLAYER_HPP__

#include "GameEntity.hpp"
#include "../Core/Debouncer.hpp"

class Player : public GameEntity
{
public:
    Player();
    ~Player();

    const float speedPxPerSecond = 100.0f;

    void update(Level& level);
    void draw(Camera& camera);

    Debouncer hurtDebouncer = Debouncer(500);
    void receiveDamage(const int amount);
};

#endif // __SRC_GAMEENTITY_PLAYER_HPP__
