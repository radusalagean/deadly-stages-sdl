#ifndef __SRC_CORE_FLOATANIMATOR_HPP__
#define __SRC_CORE_FLOATANIMATOR_HPP__

#include <chrono>

class FloatAnimator {
public:
    FloatAnimator(float& value, float targetValue, int durationMs);
    
    void update();
    bool isComplete() const;

private:
    float& value;
    float startValue;
    float targetValue;
    int durationMs;
    int elapsedTimeMs;
};

#endif // __SRC_CORE_FLOATANIMATOR_HPP__