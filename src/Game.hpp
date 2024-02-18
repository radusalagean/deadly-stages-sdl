#ifndef DEADLYSTAGES_GAME_HPP
#define DEADLYSTAGES_GAME_HPP

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
    extern SDL_Event event;

    // Screen
    extern Screen* screen;

    // Game controller
    extern SDL_GameController* gameController;

    void init();
    void loop();
    void dispose();
};

#endif