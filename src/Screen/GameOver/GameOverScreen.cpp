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
    for (auto drawable : drawables)
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
    for (auto drawable : drawables)
    {
        drawable->load();
    }
}

void GameOverScreen::loadMenuItems()
{
    std::vector<MenuItemDrawable*> menuItems{
        new MenuItemDrawable("CONTINUE", std::bind(&GameOverScreen::menuContinue, this)),
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
    for (auto drawable : drawables)
    {
        drawable->update();
    }
}

void GameOverScreen::render()
{
    SDL_RenderClear(Game::renderer);
    for (auto drawable : drawables)
    {
        drawable->draw();
    }
    SDL_RenderPresent(Game::renderer);
}

void GameOverScreen::layoutPass()
{
    { // Game Over
        int height = USCALE(Game::height * 0.6);
        int width = height * gameOverImageDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        int y = USCALE(Game::height * 0.1);
        gameOverImageDrawable->layout(x, y, width, height);
    }
    { // Stats
        int height = USCALE(Game::height * 0.06);
        int width = height * statsTextDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        int y = Game::height / 2 - height / 2;
        statsTextDrawable->layout(x, y, width, height);
    }
    { // Menu
        int height = USCALE(Game::height * 0.3);
        int width = USCALE(Game::width * 0.5);
        int x = (Game::width - width) / 2;
        int y = Game::height - height - USCALE(Game::height * 0.05) - Constants::WINDOW_PADDING_PX;
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