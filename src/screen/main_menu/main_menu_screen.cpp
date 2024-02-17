#include "main_menu_screen.hpp"

MainMenuScreen::MainMenuScreen(SDL_Renderer* renderer) : Screen::Screen(renderer)
{
    
}

void MainMenuScreen::init()
{
    square = {216, 96, 34, 64};
}

bool MainMenuScreen::update()
{
    return true;
}

void MainMenuScreen::render()
{
    // Clear the screen
    SDL_RenderClear(renderer);

    // Draw a red square
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &square);

    // Draw everything on a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderPresent(renderer);
}

void MainMenuScreen::dispose()
{
    
}