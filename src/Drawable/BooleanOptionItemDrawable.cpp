#include "BooleanOptionItemDrawable.hpp"

#include "../Control/Control.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <algorithm>

BooleanOptionItemDrawable::BooleanOptionItemDrawable(const std::string& text, bool currentValue, std::function<void(bool)> callback)
    : OptionItemDrawable(text), callback(callback), currentValue(currentValue)
{
}

void BooleanOptionItemDrawable::layout(int x, int y, int w, int h)
{
    OptionItemDrawable::layout(x, y, w, h);
    checkboxRect.h = h;
    checkboxRect.w = h;
    checkboxRect.x = x + w - checkboxRect.w;
    checkboxRect.y = y;
    checkboxOutlineThickness = std::max(1, static_cast<int>(h * 0.2f));
}

void BooleanOptionItemDrawable::load()
{
    OptionItemDrawable::load();
}

void BooleanOptionItemDrawable::handleEvents()
{
    OptionItemDrawable::handleEvents();
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_MENUS_SELECT, &pressedActionData))
    {
        toggle();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void BooleanOptionItemDrawable::update()
{
    OptionItemDrawable::update();
}

void BooleanOptionItemDrawable::draw()
{
    OptionItemDrawable::draw();
    SDL_Color color = selected ? Constants::COLOR_YELLOW_ACCENT : Constants::COLOR_WHITE;
    if (currentValue)
    {
        Game::primitiveShapeHelper.drawRect(checkboxRect, color);
    }
    else
    {
        Game::primitiveShapeHelper.drawRectOutline(checkboxRect, color, checkboxOutlineThickness);
    }
}

void BooleanOptionItemDrawable::toggle()
{
    currentValue = !currentValue;
    callback(currentValue);
}