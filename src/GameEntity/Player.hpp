#ifndef __SRC_GAMEENTITY_PLAYER_HPP__
#define __SRC_GAMEENTITY_PLAYER_HPP__

#include "GameEntity.hpp"

class Player : public GameEntity
{
public:
    Player();
    ~Player();

    const float speedPxPerSeconds = 100.0f;
};

#endif // __SRC_GAMEENTITY_PLAYER_HPP__
