#ifndef DEADLYSTAGES_GAME_HPP
#define DEADLYSTAGES_GAME_HPP

#include <SDL2/SDL.h>
#include "screen/screen.hpp"
#include "screen/main_menu/main_menu_screen.hpp"

namespace Game
{
    // Flags
    extern bool isRunning;

    // SDL
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_Event event;

    // Screen
    extern Screen* screen;

    void init();
    void loop();
    void dispose();
};

#endif