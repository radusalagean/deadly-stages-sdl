#ifndef __SRC_DRAWABLE_SLIDEROPTIONITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_SLIDEROPTIONITEMDRAWABLE_HPP__

#include "OptionItemDrawable.hpp"

#include <SDL.h>
#include <string>
#include <functional>
#include "../Core/Debouncer.hpp"

class SliderOptionItemDrawable : public OptionItemDrawable
{
private:
    constexpr static float SLIDER_STEP_VALUE = 0.1f;

    std::function<void(float)> callback;
    SDL_Rect sliderRect;
    SDL_Rect sliderFullRect;
    float sliderValue;
    void refreshSliderRectWidth();
    void onSliderValueChangeRequest(float offset);
    Debouncer navigationDebouncer;

    enum NavigationAction
    {
        NAVIGATION_ACTION_LEFT = 0,
        NAVIGATION_ACTION_RIGHT
    };

public:
    SliderOptionItemDrawable(const std::string& text, float currentValue, std::function<void(float)> callback);
    void layout(int x, int y, int w, int h);
    void load();
    void handleEvents();
    void update();
    void draw();
};

#endif // __SRC_DRAWABLE_SLIDEROPTIONITEMDRAWABLE_HPP__
