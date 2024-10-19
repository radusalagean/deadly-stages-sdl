#ifndef __SRC_LEVEL_CAMERA_HPP__
#define __SRC_LEVEL_CAMERA_HPP__

#include "../Core/Vector2D.hpp"
#include <chrono>

class GameEntity;

class Camera
{
public:
    Vector2D position;
    Vector2D* target = nullptr;
    Vector2D* mapBounds = nullptr;

    void update();
    
    void startShake(unsigned int durationMs, float intensity);
    void updateShake();

private:
    bool isShaking = false;
    std::chrono::steady_clock::time_point shakeStartTime;
    int shakeDuration = 0;
    float shakeIntensity = 0.0f;
    Vector2D shakeOffset;
};

#endif // __SRC_LEVEL_CAMERA_HPP__
