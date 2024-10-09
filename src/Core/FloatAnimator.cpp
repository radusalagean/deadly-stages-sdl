#include "FloatAnimator.hpp"
#include <algorithm>
#include "../Game.hpp"

FloatAnimator::FloatAnimator(float& value, float targetValue, int durationMs)
    : value(value), startValue(value), targetValue(targetValue), durationMs(durationMs), elapsedTimeMs(0)
{
}

void FloatAnimator::update()
{
    if (isComplete()) {
        return;
    }

    elapsedTimeMs += static_cast<int>(Game::latestLoopDeltaTimeMs);
    float progress = std::min(1.0f, static_cast<float>(elapsedTimeMs) / durationMs);
    
    value = startValue + (targetValue - startValue) * progress;
}

bool FloatAnimator::isComplete() const
{
    return elapsedTimeMs >= durationMs;
}