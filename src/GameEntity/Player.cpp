#include "Player.hpp"

#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <math.h>
#include <cmath>
#include "../Core/Config.hpp"
#include "../Level/Level.hpp"
#include "../Core/CollisionManager.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../GameEntity/Pickup.hpp"
#include "../Core/Config.hpp"

Player::Player() : GameEntity()
{
    texturePath = "res/game_entity/player.png";
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

        CollisionManager::processMovement(*this, CollisionManager::EntityType::PLAYER, velocity, level, isJumping());
        this->velocity = velocity;
    }
    else
    {
        this->velocity.reset();
    }

    // Aiming
    #ifdef SUPPORTS_AIM_ASSIST
    const PressedActionData* pressedActionData = nullptr;
    int scanDirection = 0;
    if (Game::control.isActionDown(CA_GAME_AIM_ASSIST_LEFT, &pressedActionData))
    {
        scanDirection = -1;
    }
    else if (Game::control.isActionDown(CA_GAME_AIM_ASSIST_RIGHT, &pressedActionData))
    {
        scanDirection = 1;
    }
    if (scanDirection != 0)
    {
        if (Game::control.aimAssist)
        {
            assignTargetToNearestEnemy(level, scanDirection);
        }
        else
        {
            targetEnemy = nullptr;
        }
        if (targetEnemy == nullptr)
        {
            rotation += rotationSpeedDegreesPerSecond * scanDirection * Game::latestLoopDeltaTimeSeconds;
        }
        else
        {
            Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
        }
    }
    #else
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
    #endif
}

void Player::update(Level& level)
{
    #ifdef SUPPORTS_AIM_ASSIST
    if (targetEnemy != nullptr)
    {
        rotateToTarget(targetEnemy->positionPlusCenter);
    }
    #endif
    GameEntity::update(level);
    updateJumpState(level);
    crushEnemiesIfNeeded(level);
    collectPickupIfNeeded(level);
    increaseStaminaIfPossible();
    #ifdef SUPPORTS_AIM_ASSIST
    loseTargetIfNotVisible(level);
    if (pendingAimAssistScan)
    {
        assignTargetToNearestEnemy(level, 0);
        pendingAimAssistScan = false;
    }
    #endif
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
    Game::audioManager.playSound(AudioSFXId::PLAYER_HURT);
}

void Player::onJumpRequest()
{
    if (isJumping() || stamina < jumpStaminaCost) {
        return;
    }
    stamina -= jumpStaminaCost;
    lastStaminaDecreaseTime = std::chrono::steady_clock::now();
    jumpAnimator = new FloatAnimator(dstRectScale, maxDstRectScale, jumpDurationMs);
    Game::audioManager.playSound(AudioSFXId::JUMP);
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
        Game::audioManager.playSound(AudioSFXId::LAND_FROM_JUMP);
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
            Game::audioManager.playSound(AudioSFXId::CRUSH);
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

#ifdef SUPPORTS_AIM_ASSIST
/**
 * scanDirection: 
 *  -1 for 15 degree scan to the left, 
 *   1 for 15 degree scan to the right, 
 *   0 for a -15 to 15 degree scan
 */
void Player::assignTargetToNearestEnemy(Level& level, const int scanDirection)
{
    Enemy* nearestEnemy = nullptr;
    float nearestDistance = std::numeric_limits<float>::max();
    float playerAngleDegrees = fmod(rotation + 180, 360.0f);
    if (playerAngleDegrees < 0) 
        playerAngleDegrees += 360.0f;
    float playerAngleRad = playerAngleDegrees * M_PI / 180.0f;
    const float scanAngleRad = 15.0f * M_PI / 180.0f;
    
    for (auto& enemy : level.enemies)
    {
        if (!level.camera.isDstRectVisible(enemy->dstRect) || enemy == targetEnemy)
            continue;
        Vector2D toEnemy = enemy->positionPlusCenter - positionPlusCenter;
        float distance = toEnemy.magnitude();
        float angle = atan2(toEnemy.y, toEnemy.x);
        
        // Normalize the angle difference to be between -PI and PI
        float angleDiff = angle - playerAngleRad;
        while (angleDiff > M_PI) angleDiff -= 2 * M_PI;
        while (angleDiff < -M_PI) angleDiff += 2 * M_PI;
        
        // Check if the enemy is within the scan range
        if ((scanDirection == 0 && std::abs(angleDiff) <= scanAngleRad) ||
            (scanDirection == -1 && angleDiff >= -scanAngleRad && angleDiff <= 0) ||
            (scanDirection == 1 && angleDiff >= 0 && angleDiff <= scanAngleRad))
        {
            if (distance < nearestDistance)
            {
                nearestDistance = distance;
                nearestEnemy = enemy;
            }
        }
    }
    targetEnemy = nearestEnemy;
}

void Player::loseTargetIfNotVisible(Level& level)
{
    if (targetEnemy == nullptr)
        return;
    if (!level.camera.isDstRectVisible(targetEnemy->dstRect))
    {
        targetEnemy = nullptr;
        pendingAimAssistScan = true;
    }
}

void Player::onTargetEnemyRemoved(Level& level)
{
    targetEnemy = nullptr;
    pendingAimAssistScan = true;
}
#endif
