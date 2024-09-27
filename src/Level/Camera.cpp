#include "Camera.hpp"

#include "../Game.hpp"
#include <algorithm>

void Camera::update()
{
    if (target != nullptr)
    {
        // Calculate new position
        float newX = target->getX() - Game::width / 2;
        float newY = target->getY() - Game::height / 2;

        // Map bounds
        float boundsX = std::max(0.0f, mapBounds->getX() - Game::width);
        float boundsY = std::max(0.0f, mapBounds->getY() - Game::height);

        // Clamp the new position to the map bounds
        newX = std::max(0.0f, std::min(newX, boundsX));
        newY = std::max(0.0f, std::min(newY, boundsY));

        // Update the camera position
        position.setX(newX);
        position.setY(newY);
    }
}