#ifndef __SRC_GAME_HPP__
#define __SRC_GAME_HPP__

#include "Core/Config.hpp"
#include <SDL.h>
#include "Screen/Screen.hpp"
#include "Screen/MainMenu/MainMenuScreen.hpp"
#include "Debug/Logger.hpp"
#include "Control/Control.hpp"
#include "Core/AudioManager.hpp"
#include <string>

extern std::string resDirPathPrefix;

extern void platformSaveOptions();
extern void platformLoadOptions();

class FontManager;
class ScreenManager;
class PrimitiveShapeHelper;

namespace Game 
{
    // Flags
    extern bool isRunning;
    extern bool isLoading;
    extern bool isSaving;

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
    extern AudioManager audioManager;

    // Time
    extern float latestLoopDeltaTimeMs;
    extern float latestLoopDeltaTimeSeconds;

    // Framerate
    extern std::vector<int> framerateLimitOptions;
    extern std::vector<std::string> framerateLimitOptionLabels;
    extern bool showFramerate;
    extern float minSecondsPerFrame;
    void setFramerateLimitIndex(int index);
    const int getFramerateLimitIndex();

    void init();

    void loop();
    void handleEvents();
    void update();
    void render();

    void dispose();

    void syncRendererOutputSize();
};

#endif // __SRC_GAME_HPP__