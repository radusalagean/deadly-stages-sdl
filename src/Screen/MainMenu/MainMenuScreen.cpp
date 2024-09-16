#include "MainMenuScreen.hpp"

#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Controls/Controls.hpp"
#include "../../Core/Constants.hpp"
#include "../../Drawable/MenuItemDrawable.hpp"
#include "../LevelSelect/LevelSelectScreen.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    titleScreenImageDrawable = new ImageDrawable("title_screen.png");
    drawables.push_back(titleScreenImageDrawable);
    demoLabelTextDrawable = new TextDrawable("DEMO");
    drawables.push_back(demoLabelTextDrawable);
    copyrightLabelTextDrawable = new TextDrawable("© 2015-2024 ShadowzGames");
    drawables.push_back(copyrightLabelTextDrawable);
    mainMenuDrawable = new MenuDrawable();
    drawables.push_back(mainMenuDrawable);
}

void MainMenuScreen::init()
{
    loadMenuItems();
    loadAssets();
}

void MainMenuScreen::handleEvents()
{
    if (Game::controls.isActionDown(CA_ESCAPE))
        Game::isRunning = false;
    mainMenuDrawable->handleEvents();
}

void MainMenuScreen::loadAssets()
{
    for (auto drawable : drawables)
    {
        drawable->load();
    }
}

void MainMenuScreen::loadMenuItems()
{
    std::vector<MenuItemDrawable*> menuItems{
        new MenuItemDrawable("START GAME", std::bind(&MainMenuScreen::menuStartGame, this)),
        new MenuItemDrawable("OPTIONS", std::bind(&MainMenuScreen::menuOptions, this)),
        new MenuItemDrawable("CREDITS", std::bind(&MainMenuScreen::menuCredits, this)),
        new MenuItemDrawable("EXIT", std::bind(&MainMenuScreen::menuExit, this))
    };
    mainMenuDrawable->setMenuItems(menuItems);
}

void MainMenuScreen::update()
{
    for (auto drawable : drawables)
    {
        drawable->update();
    }
}

void MainMenuScreen::render()
{
    SDL_RenderClear(Game::renderer);
    for (auto drawable : drawables)
    {
        drawable->draw();
    }
    SDL_RenderPresent(Game::renderer);
}

void MainMenuScreen::dispose()
{
    for (auto drawable : drawables)
    {
        drawable->dispose();
        delete drawable;
    }
    drawables.clear();
}

void MainMenuScreen::layoutPass()
{
    { // Title Screen
        int y = Game::height * 0.1 * Constants::UI_SCALE;
        int height = Game::height * 0.6 * Constants::UI_SCALE;
        int width = height * titleScreenImageDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        titleScreenImageDrawable->layout(x, y, width, height);
    }
    { // Main Menu
        int height = Game::height * 0.3 * Constants::UI_SCALE;
        int width = Game::width * 0.5 * Constants::UI_SCALE;
        int x = (Game::width - width) / 2;
        int y = Game::height - height - (Game::height * 0.05 * Constants::UI_SCALE) - Constants::WINDOW_PADDING_PX;
        mainMenuDrawable->layout(x, y, width, height);
    }
    { // Demo Label
        int height = Game::height * 0.04 * Constants::UI_SCALE;
        int width = height * demoLabelTextDrawable->getAspectRatio();
        int x = Constants::WINDOW_PADDING_PX;
        int y = Game::height - height - Constants::WINDOW_PADDING_PX;
        demoLabelTextDrawable->layout(x, y, width, height);
    }
    { // Copyright Label
        int height = Game::height * 0.04 * Constants::UI_SCALE;
        int width = height * copyrightLabelTextDrawable->getAspectRatio();
        int x = Game::width - width - Constants::WINDOW_PADDING_PX;
        int y = Game::height - height - Constants::WINDOW_PADDING_PX;
        copyrightLabelTextDrawable->layout(x, y, width, height);
    }
    layoutInvalidated = false;
}

void MainMenuScreen::menuStartGame()
{
    Game::screenManager.pushScreen(new LevelSelectScreen());
}

void MainMenuScreen::menuOptions()
{
    logd("Options");
}

void MainMenuScreen::menuCredits()
{
    logd("Credits");
}

void MainMenuScreen::menuExit()
{
   Game::isRunning = false;
}