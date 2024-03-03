#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    titleScreenImageAsset = new ImageAsset("res/image/title_screen.png",
        SDL_Rect { 0, 0, 20, 20 });
}

void MainMenuScreen::init()
{
    IMG_Init(IMG_INIT_PNG);
    screenSurface = SDL_GetWindowSurface(Game::window);
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
    
}