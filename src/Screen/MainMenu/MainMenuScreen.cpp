#include "MainMenuScreen.hpp"

#include "../../Game.hpp"
#include "../../Controls/Controls.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    titleScreenImageAsset = new ImageAsset("res/image/title_screen.png");
    drawables.push_back(titleScreenImageAsset);
}

void MainMenuScreen::init()
{
    loadAssets();
}

void MainMenuScreen::handleEvents()
{
    if (Controls::isKeyDown(SDL_SCANCODE_ESCAPE) || Controls::isGameControllerButtonDown(SDL_CONTROLLER_BUTTON_START))
        Game::isRunning = false;
}

void MainMenuScreen::loadAssets()
{
    for (auto drawable : drawables)
    {
        drawable->load();
    }
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
        int y = Game::height * 0.1;
        int height = Game::height * 0.6;
        int width = height * titleScreenImageAsset->getAspectRatio();
        int x = (Game::width - width) / 2;
        titleScreenImageAsset->layout(x, y, width, height);
    }
    layoutInvalidated = false;
}