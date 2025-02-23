#ifndef __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__
#define __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "../../Drawable/ImageDrawable.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Drawable/MenuDrawable.hpp"
#include "../Screen.hpp"

class MainMenuScreen : public Screen
{
private:
    std::vector<Drawable*> drawables{};
    
    #pragma region Drawables
    ImageDrawable* titleScreenImageDrawable = nullptr;
    TextDrawable* versionLabelTextDrawable = nullptr;
    TextDrawable* copyrightLabelTextDrawable = nullptr;
    MenuDrawable* mainMenuDrawable = nullptr;
    #pragma endregion

    void loadAssets();
    void loadMenuItems();

    void menuStartGame();
    void menuViewControls();
    void menuOptions();
    void menuCredits();
    void menuExit();
protected:
public:
    MainMenuScreen();
    ~MainMenuScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_MAINMENU_MAINMENUSCREEN_HPP__