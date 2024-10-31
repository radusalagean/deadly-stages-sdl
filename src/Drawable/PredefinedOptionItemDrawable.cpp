#include "PredefinedOptionItemDrawable.hpp"

#include "../Control/Control.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

PredefinedOptionItemDrawable::PredefinedOptionItemDrawable(const std::string& text, const std::vector<std::string>& options, 
    int selectedOptionIndex, int defaultOptionIndex, std::function<void(int)> callback)
    : OptionItemDrawable(text), callback(callback), options(options), selectedOptionIndex(selectedOptionIndex), 
    defaultOptionIndex(defaultOptionIndex)
{
}

void PredefinedOptionItemDrawable::layout(int x, int y, int w, int h)
{
    OptionItemDrawable::layout(x, y, w, h);
    layoutRect.x = x;
    layoutRect.y = y;
    layoutRect.w = w - contentPadding;
    layoutRect.h = h;
    refreshValueTextLayout();
}

void PredefinedOptionItemDrawable::load()
{
    OptionItemDrawable::load();
    valueTextDrawable.setText(options[selectedOptionIndex]);
    valueTextDrawable.load();
}

void PredefinedOptionItemDrawable::handleEvents()
{
    OptionItemDrawable::handleEvents();
    if (Game::control.isActionDown(CA_MENUS_LEFT))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_LEFT))
        {
            onNavigationAction(NAVIGATION_ACTION_LEFT);
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_LEFT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_LEFT);
    }

    if (Game::control.isActionDown(CA_MENUS_RIGHT) || Game::control.isActionDown(CA_MENUS_SELECT))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_RIGHT))
        {
            onNavigationAction(NAVIGATION_ACTION_RIGHT);
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_RIGHT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_RIGHT);
    }
}

void PredefinedOptionItemDrawable::update()
{
    OptionItemDrawable::update();
    if (selected)
    {
        valueTextDrawable.setColor(Constants::COLOR_YELLOW_ACCENT);
    }
    else
    {
        valueTextDrawable.setColor(Constants::COLOR_WHITE);
    }
    valueTextDrawable.update();
}

void PredefinedOptionItemDrawable::draw()
{
    OptionItemDrawable::draw();
    valueTextDrawable.draw();
}

void PredefinedOptionItemDrawable::refreshValueTextLayout()
{
    int valueTextHeight = layoutRect.h;
    int valueTextWidth = layoutRect.h * valueTextDrawable.getAspectRatio();
    int valueTextX = layoutRect.x + layoutRect.w - valueTextWidth;
    int valueTextY = layoutRect.y;
    valueTextDrawable.layout(valueTextX, valueTextY, valueTextWidth, valueTextHeight);
}

void PredefinedOptionItemDrawable::onNavigationAction(NavigationAction action)
{
    if (action == NAVIGATION_ACTION_LEFT)
    {
        selectedOptionIndex = (selectedOptionIndex - 1 + options.size()) % options.size();
    }
    else if (action == NAVIGATION_ACTION_RIGHT)
    {
        selectedOptionIndex = (selectedOptionIndex + 1) % options.size();
    }
    sync();
}

void PredefinedOptionItemDrawable::restoreDefaultValue()
{
    selectedOptionIndex = defaultOptionIndex;
    sync();
}

void PredefinedOptionItemDrawable::sync()
{
    valueTextDrawable.setText(options[selectedOptionIndex]);
    refreshValueTextLayout();
    callback(selectedOptionIndex);
}