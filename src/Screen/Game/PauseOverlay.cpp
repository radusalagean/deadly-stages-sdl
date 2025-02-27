#include "PauseOverlay.hpp"

#include "../../Drawable/MenuDrawable.hpp"
#include "../../Drawable/MenuItemDrawable.hpp"
#include "../../Screen/MainMenu/MainMenuScreen.hpp"
#include "../../Screen/Controls/ControlsScreen.hpp"
#include "../Options/OptionsScreen.hpp"
#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"
#include "../../Core/Macros.hpp"

PauseOverlay::PauseOverlay(bool& paused)
    : paused(paused)
{
    menuDrawable = new MenuDrawable();
}

PauseOverlay::~PauseOverlay()
{
    delete menuDrawable;
}

void PauseOverlay::load()
{
    std::vector<MenuItemDrawable*> menuItems{
        new MenuItemDrawable("CONTINUE", std::bind(&PauseOverlay::menuContinue, this)),
        new MenuItemDrawable("VIEW CONTROLS", std::bind(&PauseOverlay::menuViewControls, this)),
        new MenuItemDrawable("OPTIONS", std::bind(&PauseOverlay::menuOptions, this)),
        new MenuItemDrawable("QUIT TO MAIN MENU", std::bind(&PauseOverlay::menuQuitToMainMenu, this))
    };
    menuDrawable->setMenuItems(menuItems);
    menuDrawable->load();
}

void PauseOverlay::layoutPass()
{
    // Background
    backgroundRect = {0, 0, Game::width, Game::height};
    // Menu
    {
        int height = USCALE(Game::height * 0.3);
        int width = USCALE(Game::width * 0.5);
        int x = (Game::width - width) / 2;
        int y = (Game::height - height) / 2;
        menuDrawable->layout(x, y, width, height);
    }
}

void PauseOverlay::update()
{
    menuDrawable->update();
}

void PauseOverlay::render()
{
    Game::primitiveShapeHelper.drawRect(backgroundRect, {0, 0, 0, 100});
    menuDrawable->draw();
}

void PauseOverlay::handleEvents()
{
    menuDrawable->handleEvents();
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_MENUS_BACK, &pressedActionData))
    {
        menuContinue();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void PauseOverlay::menuContinue()
{
    paused = false;
    Game::audioManager.resumeMusic();
}


void PauseOverlay::menuViewControls()
{
    Game::screenManager.pushScreen(new ControlsScreen());
}

void PauseOverlay::menuOptions()
{
    Game::screenManager.pushScreen(new OptionsScreen());
}

void PauseOverlay::menuQuitToMainMenu()
{
    Game::screenManager.setScreen(new MainMenuScreen());
}