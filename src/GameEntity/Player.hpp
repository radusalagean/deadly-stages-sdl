#ifndef __SRC_GAMEENTITY_PLAYER_HPP__
#define __SRC_GAMEENTITY_PLAYER_HPP__

#include "GameEntity.hpp"
#include "../Core/Debouncer.hpp"
#include "../Core/FloatAnimator.hpp"

class Player : public GameEntity
{
public:
    Player();
    ~Player();

    void update(Level& level);
    void draw(Camera& camera);

    // Speed
    float speedPxPerSecond = 100.0f;

    // Damage
    Debouncer hurtDebouncer = Debouncer(500);
    void receiveDamage(const int amount, Level& level);

    // Stamina
    const float staminaDecreaseRatePerSecond = 0.5f;
    const float staminaIncreaseRatePerSecond = 0.2f;
    const int staminaIncreaseCooldownMs = 2000;
    float stamina = 1.0f;
    std::chrono::steady_clock::time_point lastStaminaDecreaseTime = std::chrono::steady_clock::now();
    bool canIncreaseStamina();
    void increaseStaminaIfPossible();

    // Sprint
    const float sprintBoost = 100.0f;

    // Jump
    const int jumpDurationMs = 500;
    const float maxDstRectScale = 1.12f;
    FloatAnimator* jumpAnimator = nullptr;
    const float jumpStaminaCost = 0.2f;
    void onJumpRequest();
    void updateJumpState(Level& level);
    inline bool isJumping() { return jumpAnimator != nullptr; }
    void crushEnemiesIfNeeded(Level& level);
    void collectPickupIfNeeded(Level& level);
};

#endif // __SRC_GAMEENTITY_PLAYER_HPP__
