#include "Player.hpp"

#include "../Core/Macros.hpp"
#include <math.h>

Player::Player() : GameEntity()
{

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

    // Calculate the angle between the player and the mouse relative to the camera position
    float angle = atan2(mouseY - GSCALE(position.getY()) + camera.position.getY(), mouseX - GSCALE(position.getX()) + camera.position.getX());

    // Convert the angle from radians to degrees
    angle = angle * (180 / M_PI);

    // Set the rotation of the player
    rotation = angle - 180;
}
