#include "ClickableOptionItemDrawable.hpp"

#include "../Control/Control.hpp"
#include "../Game.hpp"

ClickableOptionItemDrawable::ClickableOptionItemDrawable(const std::string& text, std::function<void()> callback)
    : OptionItemDrawable(text), callback(callback)
{
}

void ClickableOptionItemDrawable::layout(int x, int y, int w, int h)
{
    OptionItemDrawable::layout(x, y, w, h);
}

void ClickableOptionItemDrawable::load()
{
    OptionItemDrawable::load();
}

void ClickableOptionItemDrawable::handleEvents()
{
    OptionItemDrawable::handleEvents();
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_MENUS_SELECT, &pressedActionData))
    {
        callback();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void ClickableOptionItemDrawable::update()
{
    OptionItemDrawable::update();
}

void ClickableOptionItemDrawable::draw()
{
    OptionItemDrawable::draw();
}
