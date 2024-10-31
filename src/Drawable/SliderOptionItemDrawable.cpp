#include "SliderOptionItemDrawable.hpp"

#include "../Control/Control.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

SliderOptionItemDrawable::SliderOptionItemDrawable(const std::string& text, float currentValue, std::function<void(float)> callback)
    : OptionItemDrawable(text), callback(callback), sliderValue(currentValue)
{
}

void SliderOptionItemDrawable::layout(int x, int y, int w, int h)
{
    OptionItemDrawable::layout(x, y, w, h);
    sliderFullRect.w = w * 0.3f;
    sliderFullRect.h = h;
    sliderFullRect.x = x + w - sliderFullRect.w - contentPadding;
    sliderFullRect.y = y;
    sliderRect = sliderFullRect;
    sliderOutlineThickness = std::max(1, static_cast<int>(h * 0.2f));
    refreshSliderRectWidth();
}

void SliderOptionItemDrawable::load()
{
    OptionItemDrawable::load();
}

void SliderOptionItemDrawable::handleEvents()
{
    OptionItemDrawable::handleEvents();
    if (Game::control.isActionDown(CA_MENUS_LEFT))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_LEFT))
        {
            onSliderValueChangeRequest(-SLIDER_STEP_VALUE);
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_LEFT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_LEFT);
    }

    if (Game::control.isActionDown(CA_MENUS_RIGHT))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_RIGHT))
        {
            onSliderValueChangeRequest(SLIDER_STEP_VALUE);
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_RIGHT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_RIGHT);
    }
}

void SliderOptionItemDrawable::update()
{
    OptionItemDrawable::update();
}

void SliderOptionItemDrawable::draw()
{
    OptionItemDrawable::draw();
    SDL_Color color = selected ? Constants::COLOR_YELLOW_ACCENT : Constants::COLOR_WHITE;
    Game::primitiveShapeHelper.drawRect(sliderRect, color);
    Game::primitiveShapeHelper.drawRectOutline(sliderFullRect, color, sliderOutlineThickness);
}

void SliderOptionItemDrawable::refreshSliderRectWidth()
{
    sliderRect.w = sliderFullRect.w * sliderValue;
}

void SliderOptionItemDrawable::onSliderValueChangeRequest(float offset)
{
    sliderValue = std::min(std::max(sliderValue + offset, 0.0f), 1.0f);
    refreshSliderRectWidth();
    callback(sliderValue);
}
