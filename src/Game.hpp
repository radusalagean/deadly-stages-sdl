#ifndef __SRC_GAME_HPP__
#define __SRC_GAME_HPP__

#include <SDL.h>
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
    extern int width;
    extern int height;

    void init();

    void loop();
    void handleEvents();
    void update();
    void render();

    void dispose();

    void syncRendererOutputSize();
};

#endif // __SRC_GAME_HPP__