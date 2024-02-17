#ifndef DEADLYSTAGES_MAIN_MENU_SCREEN_HPP
#define DEADLYSTAGES_MAIN_MENU_SCREEN_HPP

#include <SDL2/SDL.h>
#include "../screen.hpp"

class MainMenuScreen : public Screen
{
private:
    SDL_Rect square;
protected:
public:
    MainMenuScreen(SDL_Renderer* renderer);
    void init();
    bool update();
    void render();
    void dispose();
};

#endif