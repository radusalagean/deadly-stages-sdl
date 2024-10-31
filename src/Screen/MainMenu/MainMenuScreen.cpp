#include "MainMenuScreen.hpp"

#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Control/Control.hpp"
#include "../../Core/Constants.hpp"
#include "../../Drawable/MenuItemDrawable.hpp"
#include "../LevelSelect/LevelSelectScreen.hpp"
#include "../Options/OptionsScreen.hpp"
#include "../Credits/CreditsScreen.hpp"
#include "../../Core/Macros.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    titleScreenImageDrawable = new ImageDrawable("title_screen.png");
    drawables.push_back(titleScreenImageDrawable);
    versionLabelTextDrawable = new TextDrawable(Constants::VERSION);
    drawables.push_back(versionLabelTextDrawable);
    copyrightLabelTextDrawable = new TextDrawable("© 2015-2024 ShadowzGames");
    drawables.push_back(copyrightLabelTextDrawable);
    mainMenuDrawable = new MenuDrawable();
    drawables.push_back(mainMenuDrawable);
}

MainMenuScreen::~MainMenuScreen()
{
    for (auto& drawable : drawables)
    {
        delete drawable;
    }
    drawables.clear();
}

void MainMenuScreen::init()
{
    loadMenuItems();
    loadAssets();
    Game::audioManager.loadAndStartMusic(AudioMusicId::MENUS);
}

void MainMenuScreen::handleEvents()
{
    mainMenuDrawable->handleEvents();
}

void MainMenuScreen::loadAssets()
{
    for (auto& drawable : drawables)
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
    for (auto& drawable : drawables)
    {
        drawable->update();
    }
}

void MainMenuScreen::render()
{
    
    for (auto& drawable : drawables)
    {
        drawable->draw();
    }
}

void MainMenuScreen::layoutPass()
{
    { // Title Screen
        int y = USCALE(Game::height * 0.1);
        int height = USCALE(Game::height * 0.5);
        int width = height * titleScreenImageDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        titleScreenImageDrawable->layout(x, y, width, height);
    }
    { // Main Menu
        int titleScreenBottomY = titleScreenImageDrawable->dstRect.y + titleScreenImageDrawable->dstRect.h;
        int remainingHeight = Game::height - titleScreenBottomY;
        int height = remainingHeight;
        int width = Game::width;
        int x = 0;
        int y = titleScreenBottomY;
        mainMenuDrawable->layout(x, y, width, height);
    }
    #ifdef PLATFORM_PSP
        const float textHeight = USCALE(Game::height * 0.04);
    #else
        const float textHeight = USCALE(Game::height * 0.035);
    #endif
    { // Version Label
        int height = textHeight;
        int width = height * versionLabelTextDrawable->getAspectRatio();
        int x = Constants::WINDOW_PADDING_PX;
        int y = Game::height - height - Constants::WINDOW_PADDING_PX;
        versionLabelTextDrawable->layout(x, y, width, height);
    }
    { // Copyright Label
        int height = textHeight;
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
    Game::screenManager.pushScreen(new OptionsScreen());
}

void MainMenuScreen::menuCredits()
{
    Game::screenManager.pushScreen(new CreditsScreen());
}

void MainMenuScreen::menuExit()
{
   Game::isRunning = false;
}