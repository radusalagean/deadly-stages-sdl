#include "Camera.hpp"

#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../GameEntity/GameEntity.hpp"
#include <algorithm>
#include <random>

void Camera::update()
{
    if (target != nullptr)
    {
        // Calculate new position
        float newX = scale(target->x) - Game::width / 2;
        float newY = scale(target->y) - Game::height / 2;

        // Map bounds
        float mapWidth = scale(mapBounds->x);
        float mapHeight = scale(mapBounds->y);
        float boundsX = std::max(0.0f, mapWidth - Game::width);
        float boundsY = std::max(0.0f, mapHeight - Game::height);

        // Clamp the new position to the map bounds
        newX = std::max(0.0f, std::min(newX, boundsX));
        newY = std::max(0.0f, std::min(newY, boundsY));

        // Update the camera position
        position.x = newX;
        position.y = newY;
    }

    updateShake();
}

void Camera::startShake(unsigned int durationMs, float intensity)
{
    Game::control.rumbleCurrentControllerIfActive({RUMBLE_DEFAULT, 0.75f, 0.75f, durationMs});
    isShaking = true;
    shakeStartTime = std::chrono::steady_clock::now();
    shakeDuration = durationMs;
    shakeIntensity = intensity;
}

void Camera::updateShake()
{
    if (!isShaking)
    {
        return;
    }

    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - shakeStartTime).count();

    if (elapsedTime >= shakeDuration)
    {
        isShaking = false;
        shakeOffset = Vector2D(0, 0);
        return;
    }

    // Calculate shake intensity based on remaining time
    float remainingIntensity = shakeIntensity * (1.0f - static_cast<float>(elapsedTime) / shakeDuration);

    // Generate random offset
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-remainingIntensity, remainingIntensity);

    shakeOffset.x = distribution(generator);
    shakeOffset.y = distribution(generator);

    // Apply shake offset to camera position
    position.x += shakeOffset.x;
    position.y += shakeOffset.y;
}

void Camera::toggleZoom()
{
    currentZoom = currentZoom == ZOOM_1_0 ? ZOOM_0_5 : ZOOM_1_0;
}