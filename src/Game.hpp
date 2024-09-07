#ifndef __SRC_GAME_HPP__
#define __SRC_GAME_HPP__

#include <SDL2/SDL.h>
#include "Screen/Screen.hpp"
#include "Screen/MainMenu/MainMenuScreen.hpp"
#include "Debug/Logger.hpp"

namespace Game 
{
    // Flags
    extern bool isRunning;

    // SDL
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern SDL_Surface* windowSurface;

    void init();

    void loop();
    void handleEvents();
    void update();
    void render();

    void dispose();

    void syncWindowSurface();
};

#endif // __SRC_GAME_HPP__