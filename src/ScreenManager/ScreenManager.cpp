#include "ScreenManager.hpp"
#include <algorithm>
#include "../Screen/Screen.hpp"
#include "../Screen/MainMenu/MainMenuScreen.hpp"
#include "../Controls/Controls.hpp"
#include "../Game.hpp"

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
        screens.back()->handleEvents();
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

    handlePendingTransactions();
}

void ScreenManager::render() 
{
    if (screens.empty())
        return;
    screens.back()->render();
}

void ScreenManager::dispose() 
{
    for (auto screen : screens) 
    {
        screen->dispose();
        delete screen;
    }
    screens.clear();
}

void ScreenManager::handlePendingTransactions() 
{
    if (clearAllScreens) 
    {
        for (auto screen : screens)
            screensToRemove.push_back(screen);
        screens.clear();
        clearAllScreens = false;
    }

    // Remove screens
    for (auto screen : screensToRemove) 
    {
        auto it = std::find(screens.begin(), screens.end(), screen);
        if (it != screens.end()) 
        {
            screen->dispose();
            screens.erase(it);
            delete screen;
            Game::controls.lock();
        }
    }
    screensToRemove.clear();

    // Add new screens
    for (auto screen : screensToAdd) 
    {
        Game::controls.lock();
        screens.push_back(screen);
        screen->init();
    }
    screensToAdd.clear();
}

void ScreenManager::onRendererOutputSizeChanged()
{
    for (auto screen : screens)
        screen->invalidateLayout();
}