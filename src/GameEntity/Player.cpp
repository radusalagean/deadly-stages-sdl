#include "Player.hpp"

#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <math.h>
#include "../Core/Config.hpp"
#include "../Level/Level.hpp"
#include "../Core/CollisionManager.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../GameEntity/Pickup.hpp"

Player::Player() : GameEntity()
{
    maxHealth = 14;
    currentHealth = maxHealth;
    castShadow = true;
}

bool Player::canIncreaseStamina()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastStaminaDecreaseTime)
        .count() >= staminaIncreaseCooldownMs;
}

void Player::increaseStaminaIfPossible()
{
    if (stamina < 1.0f && canIncreaseStamina())
    {
        stamina += staminaIncreaseRatePerSecond * Game::latestLoopDeltaTimeSeconds;
        if (stamina > 1.0f)
            stamina = 1.0f;
    }
}

Player::~Player()
{

}

void Player::update(Level& level)
{
    GameEntity::update(level);

    Camera& camera = level.camera;

    // Get the position of the mouse
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Calculate the angle between the player center point and the mouse relative to the camera position
    float angle = atan2(mouseY - GSCALE(position.y + center.y) + camera.position.y, 
        mouseX - GSCALE(position.x + center.x) + camera.position.x);

    // Convert the angle from radians to degrees
    angle = angle * (180 / M_PI) - 180;

    // Set the rotation of the player
    setRotation(angle);

    updateJumpState(level);
    crushEnemiesIfNeeded(level);
    collectPickupIfNeeded(level);
    increaseStaminaIfPossible();
}

void Player::draw(Camera& camera)
{
    GameEntity::draw(camera);

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    drawCollisionRect(camera);
    #endif
}

void Player::receiveDamage(const int amount, Level& level)
{
    if (!hurtDebouncer.canPerformAction())
        return;
    GameEntity::receiveDamage(amount, level);
}

void Player::onJumpRequest()
{
    if (isJumping() || stamina < jumpStaminaCost) {
        return;
    }
    stamina -= jumpStaminaCost;
    lastStaminaDecreaseTime = std::chrono::steady_clock::now();
    jumpAnimator = new FloatAnimator(dstRectScale, maxDstRectScale, jumpDurationMs);
}

void Player::updateJumpState(Level& level)
{
    if (!isJumping())
        return;
    jumpAnimator->update();
    if (jumpAnimator->isComplete())
    {
        delete jumpAnimator;
        jumpAnimator = nullptr;
        dstRectScale = 1.0f;
        level.camera.startShake(300, 2.0f);
    }
}

void Player::crushEnemiesIfNeeded(Level& level)
{
    if (isJumping())
        return;
    for (auto& enemy : level.enemies)
    {
        if (CollisionManager::rectVsRect(positionPlusCollisionRect, enemy->positionPlusCollisionRect))
        {
            enemy->crush(level);
        }
    }
}

void Player::collectPickupIfNeeded(Level& level)
{
    for (auto& pickup : level.pickups)
    {
        if (CollisionManager::rectVsRect(positionPlusCollisionRect, pickup->positionPlusCollisionRect))
        {
            level.collectPickup(pickup);
        }
    }
}