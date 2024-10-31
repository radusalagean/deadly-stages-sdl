#include "OptionsScreen.hpp"

#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Drawable/OptionItemDrawable.hpp"
#include "../../Drawable/SliderOptionItemDrawable.hpp"
#include "../../Drawable/BooleanOptionItemDrawable.hpp"
#include "../../Drawable/PredefinedOptionItemDrawable.hpp"
#include "../../Level/Camera.hpp"
#include "../../Core/Constants.hpp"
#include "../../Core/Config.hpp"

OptionsScreen::OptionsScreen()
{
    // SFX volume
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("SFX volume", Game::audioManager.getSoundVolumeUnitInterval(), [](float value) {
        Game::audioManager.setSoundVolume(value);
    }));

    // Music volume
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("Music volume", Game::audioManager.getMusicVolumeUnitInterval(), [](float value) {
        Game::audioManager.setMusicVolume(value);
    }));

    // Aim assist
    #ifdef SUPPORTS_AIM_ASSIST
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Aim assist", Game::control.aimAssist, [](bool value) {
        Game::control.aimAssist = value;
    }));
    #endif

    // Controller rumble
    #ifdef PLATFORM_GROUP_COMPUTER
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Controller rumble", Game::control.controllerRumble, [](bool value) {
        Game::control.controllerRumble = value;
    }));
    #endif

    // Framerate limit
    optionsItemDrawables.push_back(new PredefinedOptionItemDrawable("Framerate limit", Game::framerateLimitOptionLabels, Game::getFramerateLimitIndex(), [](int value) {
        Game::setFramerateLimitIndex(value);
    }));

    // Show framerate
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Show framerate", Game::showFramerate, [](bool value) {
        Game::showFramerate = value;
    }));

    // Default camera zoom
    optionsItemDrawables.push_back(new PredefinedOptionItemDrawable("Default camera zoom", Camera::zoomOptionLabels, Camera::defaultZoomIndex, [](int value) {
        Camera::defaultZoomIndex = value;
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
    int optionItemHeight = Game::height * 0.035f;
    int optionItemSpacing = optionItemHeight * 0.7f;
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
