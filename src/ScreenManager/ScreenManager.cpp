#include "ScreenManager.hpp"
#include <algorithm>
#include "../Screen/Screen.hpp"
#include "../Screen/MainMenu/MainMenuScreen.hpp"
#include "../Control/Control.hpp"
#include "../Game.hpp"

ScreenManager::~ScreenManager()
{
    for (auto& screen : screens) 
    {
        delete screen;
    }
    screens.clear();

    if (permanentOverlayScreen != nullptr)
    {
        delete permanentOverlayScreen;
        permanentOverlayScreen = nullptr;
    }
}

void ScreenManager::pushScreen(Screen* screen) 
{
    screensToAdd.push_back(screen);
}

void ScreenManager::popScreen() 
{
    if (!screens.empty()) 
        screensToRemove.push_back(screens.back());
}

void ScreenManager::setScreen(Screen* screen) 
{
    clearAllScreens = true;
    screensToAdd.push_back(screen);
}

Screen* ScreenManager::getCurrentScreen() 
{
    return screens.empty() ? nullptr : screens.back();
}

void ScreenManager::init() 
{
    pushScreen(new MainMenuScreen());
}

void ScreenManager::handleEvents() 
{
    if (!screens.empty())
    {
        screens.back()->handleEvents();
    }
}

void ScreenManager::update() 
{
    if (!screens.empty())
    {
        Screen* currentScreen = screens.back();
        if (currentScreen->isLayoutInvalidated())
        {
            currentScreen->layoutPass();
        }
        currentScreen->update();
    }

    if (permanentOverlayScreen != nullptr)
    {
        if (permanentOverlayScreen->isLayoutInvalidated())
        {
            permanentOverlayScreen->layoutPass();
        }
        permanentOverlayScreen->update();
    }
}

void ScreenManager::render() 
{
    if (!screens.empty())
    {
        screens.back()->render();
    }

    if (permanentOverlayScreen != nullptr)
    {
        permanentOverlayScreen->render();
    }
}

void ScreenManager::handlePendingTransactions() 
{
    if (clearAllScreens) 
    {
        for (auto& screen : screens)
            screensToRemove.push_back(screen);
        clearAllScreens = false;
    }

    // Remove screens
    for (auto& screen : screensToRemove) 
    {
        auto it = std::find(screens.begin(), screens.end(), screen);
        if (it != screens.end()) 
        {
            screens.erase(it);
            delete screen;
            Game::control.lock();
        }
    }
    screensToRemove.clear();

    // Add new screens
    for (auto& screen : screensToAdd) 
    {
        Game::control.lock();
        screens.push_back(screen);
        screen->init();
    }
    screensToAdd.clear();

    if (permanentOverlayScreen == nullptr)
    {
        permanentOverlayScreen = new PermanentOverlayScreen();
        permanentOverlayScreen->init();
    }
}

void ScreenManager::onRendererOutputSizeChanged()
{
    for (auto& screen : screens)
    {
        screen->invalidateLayout();
    }

    if (permanentOverlayScreen != nullptr)
    {
        permanentOverlayScreen->invalidateLayout();
    }
}