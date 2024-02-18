#ifndef DEADLYSTAGES_MAIN_MENU_SCREEN_HPP
#define DEADLYSTAGES_MAIN_MENU_SCREEN_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../Screen.hpp"
#include "../../Game.hpp"

class MainMenuScreen : public Screen
{
private:
    SDL_Rect square;
    SDL_Surface* gScreenSurface;
    SDL_Texture* texture;
protected:
public:
    MainMenuScreen();
    void init();
    bool update();
    void render();
    void dispose();
};

#endif