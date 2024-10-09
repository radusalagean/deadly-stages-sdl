#ifndef __SRC_SCREEN_GAME_PAUSEOVERLAY_HPP__
#define __SRC_SCREEN_GAME_PAUSEOVERLAY_HPP__

#include "../../Drawable/Drawable.hpp"
#include <vector>

class MenuDrawable;

class PauseOverlay
{
public:
    PauseOverlay(bool& paused);
    ~PauseOverlay();

    void load();
    void layoutPass();
    void update();
    void render();
    void handleEvents();

private:
    bool& paused;

    MenuDrawable* menuDrawable = nullptr;
    SDL_Rect backgroundRect;

    void menuContinue();
    void menuOptions();
    void menuQuitToMainMenu();  
};

#endif // __SRC_SCREEN_GAME_PAUSEOVERLAY_HPP__
