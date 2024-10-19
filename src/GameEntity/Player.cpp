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

void Player::handleEvents(Level& level)
{
    Camera& camera = level.camera;

    // Movement
    bool isMoving = false;
    bool isMovingWithDigitalInput = false;
    if (Game::control.isAnyActionDown(digitalMovementActions))
    {
        isMoving = true;
        isMovingWithDigitalInput = true;
    }
    else if (Game::control.isActionDown(CA_GAME_MOVE_ANALOG))
    {
        isMoving = true;
    }

    if (isMoving)
    {
        float speed = speedPxPerSecond * Game::latestLoopDeltaTimeSeconds;
        Vector2D multiplier;
        if (isMovingWithDigitalInput)
        {
            multiplier.x = Game::control.isActionDown(CA_GAME_MOVE_LEFT) ? -1.0f :
                          Game::control.isActionDown(CA_GAME_MOVE_RIGHT) ? 1.0f : 
                          0.0f;
            multiplier.y = Game::control.isActionDown(CA_GAME_MOVE_UP) ? -1.0f :
                          Game::control.isActionDown(CA_GAME_MOVE_DOWN) ? 1.0f : 
                          0.0f;
        }
        else
        {
            const Vector2D& leftStick = Game::control.getLeftStickValue();
            multiplier.x = leftStick.x;
            multiplier.y = leftStick.y;
        }
        if (Game::control.isActionDown(CA_GAME_SPRINT) && stamina > 0.0f)
        {
            speed += sprintBoost * Game::latestLoopDeltaTimeSeconds;
            stamina -= staminaDecreaseRatePerSecond * Game::latestLoopDeltaTimeSeconds;
            if (stamina < 0.0f)
                stamina = 0.0f;
            lastStaminaDecreaseTime = std::chrono::steady_clock::now();
        }
        Vector2D velocity = multiplier * speed;

        CollisionManager::processMovement(*this, velocity, level, nullptr, isJumping());
        this->velocity = velocity;
    }
    else
    {
        this->velocity.reset();
    }

    // Aiming
    if (Game::control.isActionDown(CA_GAME_AIM_ANALOG))
    {
        const Vector2D& rightStick = Game::control.getRightStickValue();
        float angle = atan2(rightStick.y, rightStick.x);
        setRotation(angle * (180 / M_PI) - 180);
    }
    else if (Game::control.getCurrentControlSource() == CS_KEYBOARD_AND_MOUSE)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        float angle = atan2(mouseY - camera.scale(position.y + center.y) + camera.position.y, 
            mouseX - camera.scale(position.x + center.x) + camera.position.x);
        setRotation(angle * (180 / M_PI) - 180);
    }
}

void Player::update(Level& level)
{
    GameEntity::update(level);
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
