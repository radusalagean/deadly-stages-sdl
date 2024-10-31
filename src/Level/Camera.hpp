#ifndef __SRC_LEVEL_CAMERA_HPP__
#define __SRC_LEVEL_CAMERA_HPP__

#include "../Core/Vector2D.hpp"
#include <chrono>
#include "../Core/FloatAnimator.hpp"
#include <vector>

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
    static constexpr float ZOOM_2_0 = 2.0f;

    static std::vector<float> zoomOptions;
    static std::vector<std::string> zoomOptionLabels;

    static int defaultZoomIndex;

    int currentZoomIndex = defaultZoomIndex;

    inline float scale(float value) const { return value * getCurrentZoom(); }

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
    float getCurrentZoom() const;
};

#endif // __SRC_LEVEL_CAMERA_HPP__
