#include "Camera.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include <algorithm>

void Camera::update()
{
    if (target != nullptr)
    {
        // Calculate new position
        float newX = GSCALE(target->getX()) - Game::width / 2;
        float newY = GSCALE(target->getY()) - Game::height / 2;

        // Map bounds
        float mapWidth = GSCALE(mapBounds->getX());
        float mapHeight = GSCALE(mapBounds->getY());
        float boundsX = std::max(0.0f, mapWidth - Game::width);
        float boundsY = std::max(0.0f, mapHeight - Game::height);

        // Clamp the new position to the map bounds
        newX = std::max(0.0f, std::min(newX, boundsX));
        newY = std::max(0.0f, std::min(newY, boundsY));

        // Update the camera position
        position.setX(newX);
        position.setY(newY);
    }
}