#ifndef __SRC_GAME_HPP__
#define __SRC_GAME_HPP__

#include <SDL.h>
#include "Screen/Screen.hpp"
#include "Screen/MainMenu/MainMenuScreen.hpp"
#include "Debug/Logger.hpp"
#include "Control/Control.hpp"
#include "Core/Options.hpp"
#include "Core/FramerateIndicator.hpp"

class FontManager;
class ScreenManager;
class PrimitiveShapeHelper;

namespace Game 
{
    // Flags
    extern bool isRunning;

    // SDL
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern int width;
    extern int height;

    // Modules
    extern Control control;
    extern FontManager fontManager;
    extern ScreenManager screenManager;
    extern PrimitiveShapeHelper primitiveShapeHelper;
    extern Options options;
    extern FramerateIndicator framerateIndicator;

    // Time
    extern float latestLoopDeltaTimeMs;
    extern float latestLoopDeltaTimeSeconds;

    void init();

    void loop();
    void handleEvents();
    void update();
    void render();

    void dispose();

    void syncRendererOutputSize();
};

#endif // __SRC_GAME_HPP__