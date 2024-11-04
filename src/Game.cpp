#include "Game.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Core/FontManager.hpp"
#include "ScreenManager/ScreenManager.hpp"
#include "Core/PrimitiveShapeHelper.hpp"
#include "Core/Constants.hpp"
#include "Core/Config.hpp"
#include "Core/DefaultOptions.hpp"

namespace Game
{
    bool isRunning = true;
    bool isLoading = false;
    bool isSaving = false;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0;
    int height = 0;
    SDL_Surface* windowSurface = nullptr;

    Control control;
    FontManager fontManager;
    ScreenManager screenManager;
    PrimitiveShapeHelper primitiveShapeHelper;
    AudioManager audioManager;

    float latestLoopDeltaTimeMs = 0;
    float latestLoopDeltaTimeSeconds = 0;

    // Framerate
    std::vector<int> framerateLimitOptions = { 30, 60 };
    std::vector<std::string> framerateLimitOptionLabels = { "30 FPS", "60 FPS" };
    int framerateLimitIndex = 1;
    int minMillisPerFrame = 0;
    float minSecondsPerFrame = 0.0f;
    bool showFramerate = false;

    #ifdef PLATFORM_GROUP_COMPUTER
    bool fullscreen = false;
    bool fullscreenChangeFailed = false;
    #endif

    void setFramerateLimitIndex(int index)
    {
        framerateLimitIndex = index;
        int currentFramerateLimit = framerateLimitOptions[framerateLimitIndex];
        minMillisPerFrame = currentFramerateLimit > 0 ? 1000 / currentFramerateLimit : 0;
        minSecondsPerFrame = minMillisPerFrame / 1000.0f;
    }

    const int getFramerateLimitIndex()
    {
        return framerateLimitIndex;
    }

    void init()
    {
        // Init Logger
        Logger::init();

        // Framerate
        setFramerateLimitIndex(DefaultOptions::FRAMERATE_LIMIT_INDEX);

        // Init Control
        control.init();

        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        window = SDL_CreateWindow(
            GAME_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            MINIMUM_WINDOW_WIDTH,
            MINIMUM_WINDOW_HEIGHT,
            SDL_WINDOW_RESIZABLE
        );

        SDL_SetWindowMinimumSize(window, MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);
        #ifdef SUPPORTS_MOUSE_POINTER
        SDL_ShowCursor(SDL_DISABLE);
        #endif

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_GetRendererOutputSize(renderer, &width, &height);
        logd("Renderer size on init: %dx%d", width, height);

        screenManager.init();
        audioManager.init();

        platformLoadOptions();
    }

    void loop()
    {
        Uint32 start = SDL_GetTicks();

        handleEvents();
        update();
        render();
        screenManager.handlePendingTransactions();

        Uint32 effectiveDeltaTimeMs = SDL_GetTicks() - start;
        if (effectiveDeltaTimeMs < minMillisPerFrame) {
            SDL_Delay(minMillisPerFrame - effectiveDeltaTimeMs);
        }
        latestLoopDeltaTimeMs = SDL_GetTicks() - start;
        latestLoopDeltaTimeSeconds = latestLoopDeltaTimeMs / 1000.0f;
    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            switch (event.type) 
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) 
                    {
                        syncRendererOutputSize();
                        logd("Renderer size changed to %dx%d", width, height);
                    }
                    break;
            }
            control.handleEvent(event);
        }
        screenManager.handleEvents();
    }

    void update()
    {
        screenManager.update();

        #ifdef PLATFORM_GROUP_COMPUTER
        if (!fullscreenChangeFailed)
        {
            int result = 0;
            if (fullscreen && !(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN))
            {
                result = SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            } 
            else if (!fullscreen && (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN))
            {
                result = SDL_SetWindowFullscreen(window, 0);
            }
            if (result != 0)
            {
                fullscreenChangeFailed = true; // Because we don't want to be stuck in a loop if something goes wrong
            }
        }
        #endif
    }   

    void render()
    {
        SDL_RenderClear(renderer);
        screenManager.render();
        SDL_RenderPresent(renderer);
    }

    void dispose()
    {
        audioManager.dispose();
        control.dispose();
    }

    void syncRendererOutputSize()
    {
        int oldWidth = width;
        int oldHeight = height;
        SDL_GetRendererOutputSize(renderer, &width, &height);
        logd("Renderer size changed from %dx%d to %dx%d", oldWidth, oldHeight, width, height);
        if (width != oldWidth || height != oldHeight)
        {
            screenManager.onRendererOutputSizeChanged();
        }
    }
}
