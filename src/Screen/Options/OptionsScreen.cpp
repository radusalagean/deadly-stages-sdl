#include "OptionsScreen.hpp"

#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Drawable/OptionItemDrawable.hpp"
#include "../../Core/Constants.hpp"

OptionsScreen::OptionsScreen()
{
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("SFX Volume", Game::audioManager.getSoundVolumeUnitInterval(), [](float value) {
        Game::audioManager.setSoundVolume(value);
    }));
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("Music Volume", Game::audioManager.getMusicVolumeUnitInterval(), [](float value) {
        Game::audioManager.setMusicVolume(value);
    }));
}

OptionsScreen::~OptionsScreen()
{
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        delete optionItemDrawable;
    }
}

void OptionsScreen::init()
{
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        optionItemDrawable->load();
    }
}

void OptionsScreen::handleEvents()
{
    if (Game::control.isActionDown(CA_MENUS_UP))
    {
        if (navigationDebouncer.canPerformAction(NavigationAction::NAVIGATION_ACTION_PREVIOUS))
        {
            selectedOptionItemIndex = (selectedOptionItemIndex - 1 + optionsItemDrawables.size()) % optionsItemDrawables.size();
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_UP))
    {
        navigationDebouncer.resetAction(NavigationAction::NAVIGATION_ACTION_PREVIOUS);
    }
    if (Game::control.isActionDown(CA_MENUS_DOWN))
    {
        if (navigationDebouncer.canPerformAction(NavigationAction::NAVIGATION_ACTION_NEXT))
        {
            selectedOptionItemIndex = (selectedOptionItemIndex + 1) % optionsItemDrawables.size();
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_DOWN))
    {
        navigationDebouncer.resetAction(NavigationAction::NAVIGATION_ACTION_NEXT);
    }
    if (Game::control.isActionDown(CA_MENUS_BACK))
    {
        Game::screenManager.popScreen();
    }
    optionsItemDrawables[selectedOptionItemIndex]->handleEvents();
}

void OptionsScreen::layoutPass()
{
    int currentY = 4 * Constants::WINDOW_PADDING_PX;
    int optionItemWidth = Game::width - Constants::WINDOW_PADDING_PX * 2;
    int optionItemHeight = Game::height * 0.05f;
    int optionItemSpacing = optionItemHeight * 0.3f;
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        optionItemDrawable->layout(Constants::WINDOW_PADDING_PX, currentY, optionItemWidth, optionItemHeight);
        currentY += optionItemHeight + optionItemSpacing;
    }
    layoutInvalidated = false;
}

void OptionsScreen::update()
{
    for (unsigned i = 0; i < optionsItemDrawables.size(); i++)
    {
        optionsItemDrawables[i]->selected = i == selectedOptionItemIndex;
        optionsItemDrawables[i]->update();
    }
}

void OptionsScreen::render()
{
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        optionItemDrawable->draw();
    }
}
