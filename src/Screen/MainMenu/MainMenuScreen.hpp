#ifndef __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__
#define __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "../../Drawable/ImageDrawable.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../Screen.hpp"

class MainMenuScreen : public Screen
{
private:
    std::vector<Drawable*> drawables{};
    
    #pragma region Drawables
    ImageDrawable* titleScreenImageDrawable = nullptr;
    TextDrawable* demoLabelTextDrawable = nullptr;
    TextDrawable* copyrightLabelTextDrawable = nullptr;
    #pragma endregion

    void loadAssets();
protected:
public:
    MainMenuScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
    void dispose();
};

#endif // __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__