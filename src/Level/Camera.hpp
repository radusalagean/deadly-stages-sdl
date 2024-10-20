#ifndef __SRC_LEVEL_CAMERA_HPP__
#define __SRC_LEVEL_CAMERA_HPP__

#include "../Core/Vector2D.hpp"
#include <chrono>
#include "../Core/FloatAnimator.hpp"

#define GSCALE(value) (value * Constants::GAMEPLAY_SCALE)

class GameEntity;

class Camera
{
public:
    Vector2D position;
    Vector2D* target = nullptr;
    Vector2D* mapBounds = nullptr;

    static constexpr float ZOOM_0_5 = 0.5f;
    static constexpr float ZOOM_1_0 = 1.0f;
    static constexpr int ZOOM_ANIMATION_DURATION_MS = 1000;
    float currentZoom = ZOOM_1_0;

    inline float scale(float value) const { return value * currentZoom; }

    void update();
    
    void startShake(unsigned int durationMs, float intensity);
    void updateShake();
    void toggleZoom();
    bool isTargetVisible(const Vector2D& targetPosition);

private:
    bool isShaking = false;
    std::chrono::steady_clock::time_point shakeStartTime;
    int shakeDuration = 0;
    float shakeIntensity = 0.0f;
    Vector2D shakeOffset;
};

#endif // __SRC_LEVEL_CAMERA_HPP__
