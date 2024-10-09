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
    const float sprintMultiplier = 2.0f;
    float stamina = 1.0f;
    const float staminaDecreaseRatePerSecond = 0.5f;
    const float staminaIncreaseRatePerSecond = 0.2f;
    const int staminaIncreaseCooldownMs = 2000;
    std::chrono::steady_clock::time_point lastStaminaDecreaseTime = std::chrono::steady_clock::now();

    bool canIncreaseStamina();

    void update(Level& level);
    void draw(Camera& camera);

    Debouncer hurtDebouncer = Debouncer(500);
    void receiveDamage(const int amount);
};

#endif // __SRC_GAMEENTITY_PLAYER_HPP__
