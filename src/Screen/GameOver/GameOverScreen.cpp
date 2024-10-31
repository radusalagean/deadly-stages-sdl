#include "GameOverScreen.hpp"

#include "../../Drawable/MenuItemDrawable.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Game.hpp"
#include "../../Screen/Game/GameScreen.hpp"
#include "../../Core/Macros.hpp"

GameOverScreen::GameOverScreen(int score, int wave, const std::string& levelId) : levelId(levelId)
{
    gameOverImageDrawable = new ImageDrawable("game_over.png");
    drawables.push_back(gameOverImageDrawable);
    statsTextDrawable = new TextDrawable("Score: " + std::to_string(score) + " | Wave: " + std::to_string(wave));
    drawables.push_back(statsTextDrawable);
    menuDrawable = new MenuDrawable();
    drawables.push_back(menuDrawable);
}

GameOverScreen::~GameOverScreen()
{
    for (auto& drawable : drawables)
    {
        delete drawable;
    }
    drawables.clear();
}

void GameOverScreen::init()
{
    loadMenuItems();
    loadAssets();
}

void GameOverScreen::loadAssets()
{
    for (auto& drawable : drawables)
    {
        drawable->load();
    }
}

void GameOverScreen::loadMenuItems()
{
    std::vector<MenuItemDrawable*> menuItems{
        new MenuItemDrawable("RESTART", std::bind(&GameOverScreen::menuContinue, this)),
        new MenuItemDrawable("END", std::bind(&GameOverScreen::menuEnd, this))
    };
    menuDrawable->setMenuItems(menuItems);
}

void GameOverScreen::handleEvents()
{
    menuDrawable->handleEvents();
}

void GameOverScreen::update()
{
    for (auto& drawable : drawables)
    {
        drawable->update();
    }
}

void GameOverScreen::render()
{
    for (auto& drawable : drawables)
    {
        drawable->draw();
    }
}

void GameOverScreen::layoutPass()
{
    { // Stats
        int height = USCALE(Game::height * 0.06);
        int width = height * statsTextDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        int y = Game::height / 2 - height / 2;
        statsTextDrawable->layout(x, y, width, height);
    }
    { // Game Over
        int statsTopY = statsTextDrawable->dstRect.y;
        int spaceAboveStats = statsTopY;
        int height = USCALE(Game::height * 0.2);
        int width = height * gameOverImageDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        int y = std::max(spaceAboveStats / 2 - height / 2, 0);
        gameOverImageDrawable->layout(x, y, width, height);
    }
    { // Menu
        int statsBottomY = statsTextDrawable->dstRect.y + statsTextDrawable->dstRect.h;
        int remainingHeight = Game::height - statsBottomY;
        int height = remainingHeight;
        int width = Game::width;
        int x = 0;
        int y = statsBottomY;
        menuDrawable->layout(x, y, width, height);
    }
    layoutInvalidated = false;
}

void GameOverScreen::menuContinue()
{
    Game::screenManager.setScreen(new GameScreen(levelId));
}

void GameOverScreen::menuEnd()
{
    Game::screenManager.setScreen(new MainMenuScreen());
}