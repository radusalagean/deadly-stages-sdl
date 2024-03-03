#ifndef DEADLYSTAGES_MAIN_MENU_SCREEN_HPP
#define DEADLYSTAGES_MAIN_MENU_SCREEN_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../Core/ImageAsset.hpp"
#include "../Screen.hpp"
#include "../../Game.hpp"

class MainMenuScreen : public Screen
{
private:
    SDL_Surface* screenSurface;
    
    #pragma region Image Assets
    ImageAsset* titleScreenImageAsset;
    #pragma endregion

    void loadAssets();
protected:
public:
    MainMenuScreen();
    void init();
    bool update();
    void render();
    void dispose();
};

#endif