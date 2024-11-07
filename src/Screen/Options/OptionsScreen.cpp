#include "OptionsScreen.hpp"

#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Drawable/OptionItemDrawable.hpp"
#include "../../Drawable/SliderOptionItemDrawable.hpp"
#include "../../Drawable/BooleanOptionItemDrawable.hpp"
#include "../../Drawable/PredefinedOptionItemDrawable.hpp"
#include "../../Drawable/ClickableOptionItemDrawable.hpp"
#include "../../Level/Camera.hpp"
#include "../../Core/Constants.hpp"
#include "../../Core/Config.hpp"
#include "../../Core/Macros.hpp"

OptionsScreen::OptionsScreen()
{
    // SFX volume
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("SFX volume", Game::audioManager.getSoundVolumeUnitInterval(), DefaultOptions::SFX_VOLUME, [](float value) {
        Game::audioManager.setSoundVolume(value);
    }));

    // Music volume
    optionsItemDrawables.push_back(new SliderOptionItemDrawable("Music volume", Game::audioManager.getMusicVolumeUnitInterval(), DefaultOptions::MUSIC_VOLUME, [](float value) {
        Game::audioManager.setMusicVolume(value);
    }));

    // Aim assist
    #ifdef SUPPORTS_AIM_ASSIST
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Aim assist", Game::control.aimAssist, DefaultOptions::AIM_ASSIST, [](bool value) {
        Game::control.aimAssist = value;
    }));
    #endif

    // Controller rumble
    #ifdef SUPPORTS_CONTROLLER_RUMBLE
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Controller rumble", Game::control.controllerRumble, DefaultOptions::CONTROLLER_RUMBLE, [](bool value) {
        Game::control.controllerRumble = value;
    }));
    #endif

    // Framerate limit
    optionsItemDrawables.push_back(new PredefinedOptionItemDrawable("Framerate limit", Game::framerateLimitOptionLabels, Game::getFramerateLimitIndex(), DefaultOptions::FRAMERATE_LIMIT_INDEX, [](int value) {
        Game::setFramerateLimitIndex(value);
    }));

    // Show framerate
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Show framerate", Game::showFramerate, DefaultOptions::SHOW_FRAMERATE, [](bool value) {
        Game::showFramerate = value;
    }));

    // Fullscreen
    #ifdef PLATFORM_GROUP_COMPUTER
    optionsItemDrawables.push_back(new BooleanOptionItemDrawable("Fullscreen", Game::fullscreen, DefaultOptions::FULLSCREEN, [](bool value) {
        Game::fullscreen = value;
    }));
    #endif

    // Default camera zoom
    optionsItemDrawables.push_back(new PredefinedOptionItemDrawable("Default camera zoom", Camera::zoomOptionLabels, Camera::defaultZoomIndex, DefaultOptions::DEFAULT_CAMERA_ZOOM_INDEX, [](int value) {
        Camera::defaultZoomIndex = value;
    }));

    // Restore defaults
    optionsItemDrawables.push_back(new ClickableOptionItemDrawable("Restore defaults", [this]() {
        restoreDefaults();
    }));
}

OptionsScreen::~OptionsScreen()
{
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        delete optionItemDrawable;
    }
    platformSaveOptions();
    Game::isSaving = false;
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
        Game::isSaving = true;
        Game::screenManager.popScreen();
    }
    optionsItemDrawables[selectedOptionItemIndex]->handleEvents();
}

void OptionsScreen::layoutPass()
{
    int currentY = 4 * Constants::WINDOW_PADDING_PX;
    int optionItemWidth = Game::width - Constants::WINDOW_PADDING_PX * 2;
    int optionItemHeight = USCALE(Game::height * 0.045f);
    int optionItemSpacing = optionItemHeight * 0.7f;
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        optionItemDrawable->layout(Constants::WINDOW_PADDING_PX, currentY, optionItemWidth, optionItemHeight);
        currentY += optionItemHeight + optionItemSpacing;
    }
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

void OptionsScreen::restoreDefaults()
{
    for (auto& optionItemDrawable : optionsItemDrawables)
    {
        optionItemDrawable->restoreDefaultValue();
    }
}