#ifndef __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__
#define __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../../Core/ImageAsset.hpp"
#include "../Screen.hpp"
#include "../../Game.hpp"

class MainMenuScreen : public Screen
{
private:
    SDL_Surface* screenSurface = nullptr;
    
    #pragma region Image Assets
    ImageAsset* titleScreenImageAsset = nullptr;
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

#endif // __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__