#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    titleScreenImageAsset = new ImageAsset("res/image/title_screen.png",
        SDL_Rect { 0, 0, 640, 480 });
}

void MainMenuScreen::init()
{
    
    loadAssets();
}

void MainMenuScreen::loadAssets()
{
    titleScreenImageAsset->load();
}

bool MainMenuScreen::update()
{
    return true;
}

void MainMenuScreen::render()
{
    SDL_RenderClear(Game::renderer);
    titleScreenImageAsset->render();
    SDL_RenderPresent(Game::renderer);
}

void MainMenuScreen::dispose()
{
    titleScreenImageAsset->dispose();
    delete titleScreenImageAsset;
}