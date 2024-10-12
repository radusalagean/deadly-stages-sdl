#ifndef __SRC_CORE_FRAMERATEINDICATOR_HPP__
#define __SRC_CORE_FRAMERATEINDICATOR_HPP__

#include "../Drawable/TextDrawable.hpp"

class FramerateIndicator
{
public:
    FramerateIndicator();
    ~FramerateIndicator();

    const int targetCycleTimeMs = 1000;

    TextDrawable* framerateTextDrawable = nullptr;
    int currentCycleFrameCount = 0;
    int currentCycleTimeMs = 0;
    Uint32 lastCycleProcessingTimeMs = 0;
    int lastCycleFramerate = 0;

    void init();
    void update();
    void render();
};

#endif // __SRC_CORE_FRAMERATEINDICATOR_HPP__
