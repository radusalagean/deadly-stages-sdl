#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen() : Screen::Screen()
{
    
}

void MainMenuScreen::init()
{
    IMG_Init(IMG_INIT_PNG);
    gScreenSurface = SDL_GetWindowSurface(Game::window);
    SDL_Surface* imageSurface = SDL_LoadBMP("title_screen.bmp");
    logd(SDL_GetError());
    texture = SDL_CreateTextureFromSurface(Game::renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    // logd(SDL_GetError());
}

bool MainMenuScreen::update()
{
    return true;
}

void MainMenuScreen::render()
{
    SDL_RenderClear(Game::renderer);
    SDL_RenderCopy(Game::renderer, texture, NULL, NULL);
    SDL_RenderPresent(Game::renderer);
}

void MainMenuScreen::dispose()
{
    
}