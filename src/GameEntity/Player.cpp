#include "Player.hpp"

#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <math.h>
#include "../Core/Config.hpp"

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
    SDL_Rect debugRect;
    debugRect.x = GSCALE(position.getX()) + GSCALE(collisionRect.x) - camera.position.getX();
    debugRect.y = GSCALE(position.getY()) + GSCALE(collisionRect.y) - camera.position.getY();
    debugRect.w = GSCALE(collisionRect.w);
    debugRect.h = GSCALE(collisionRect.h);
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, debugRect, {0, 0, 255, 255}, 2);
    #endif
}
