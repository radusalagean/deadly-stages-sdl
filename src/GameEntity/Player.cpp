#include "Player.hpp"

#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <math.h>
#include "../Core/Config.hpp"

Player::Player() : GameEntity()
{
    maxHealth = 14;
    currentHealth = maxHealth;
}

Player::~Player()
{

}

void Player::update(Camera& camera)
{
    GameEntity::update();

    // Get the position of the mouse
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Calculate the angle between the player center point and the mouse relative to the camera position
    float angle = atan2(mouseY - GSCALE(position.getY() + center.getY()) + camera.position.getY(), mouseX - GSCALE(position.getX() + center.getX()) + camera.position.getX());

    // Convert the angle from radians to degrees
    angle = angle * (180 / M_PI);

    // Set the rotation of the player
    rotation = angle - 180;
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