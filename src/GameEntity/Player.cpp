#include "Player.hpp"

#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <math.h>
#include "../Core/Config.hpp"
#include "../Level/Level.hpp"

Player::Player() : GameEntity()
{
    maxHealth = 14;
    currentHealth = maxHealth;
}

bool Player::canIncreaseStamina()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastStaminaDecreaseTime)
        .count() >= staminaIncreaseCooldownMs;
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
    angle = angle * (180 / M_PI);

    // Set the rotation of the player
    rotation = angle - 180;

    updateJumpState();
}

void Player::draw(Camera& camera)
{
    GameEntity::draw(camera);

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    drawCollisionRect(camera);
    #endif
}

void Player::receiveDamage(const int amount)
{
    if (!hurtDebouncer.canPerformAction())
        return;
    GameEntity::receiveDamage(amount);
}

void Player::onJumpRequest()
{
    if (jumpAnimator || stamina < jumpStaminaCost) {
        return;
    }
    stamina -= jumpStaminaCost;
    lastStaminaDecreaseTime = std::chrono::steady_clock::now();
    jumpAnimator = new FloatAnimator(dstRectScale, 1.12f, jumpDurationMs);
}

void Player::updateJumpState()
{
    if (!jumpAnimator)
        return;
    jumpAnimator->update();
    if (jumpAnimator->isComplete()) {
        delete jumpAnimator;
        jumpAnimator = nullptr;
        dstRectScale = 1.0f;
    }
}