#include "Game.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Core/FontManager.hpp"
#include "ScreenManager/ScreenManager.hpp"
#include "Core/PrimitiveShapeHelper.hpp"
#include "Core/Constants.hpp"
#include "Core/Config.hpp"

namespace Game
{
    bool isRunning = true;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0;
    int height = 0;
    SDL_Surface* windowSurface = nullptr;

    Control control;
    FontManager fontManager;
    ScreenManager screenManager;
    PrimitiveShapeHelper primitiveShapeHelper;
    Options options;
    FramerateIndicator framerateIndicator;

    float latestLoopDeltaTimeMs = 0;
    float latestLoopDeltaTimeSeconds = 0;

    void init()
    {
        // Init Logger
        Logger::init();

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

        // Set minimum window size
        SDL_SetWindowMinimumSize(window, MINIMUM_WINDOW_WIDTH, MINIMUM_WINDOW_HEIGHT);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_GetRendererOutputSize(renderer, &width, &height);
        logd("Renderer size on init: %dx%d", width, height);

        screenManager.init();
        framerateIndicator.init();
    }

    void loop()
    {
        Uint32 start = SDL_GetTicks();

        handleEvents();
        update();
        render();

        Uint32 effectiveDeltaTimeMs = SDL_GetTicks() - start;
        if (effectiveDeltaTimeMs < Constants::MIN_MILLIS_PER_FRAME) {
            SDL_Delay(Constants::MIN_MILLIS_PER_FRAME - effectiveDeltaTimeMs);
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
                        enforceMinimumAspectRatio();
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
        framerateIndicator.update();
    }   

    void render()
    {
        SDL_RenderClear(renderer);
        screenManager.render();
        framerateIndicator.render();
        SDL_RenderPresent(renderer);
    }

    void dispose()
    {
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

    void enforceMinimumAspectRatio()
    {
        int currentWidth, currentHeight;
        SDL_GetWindowSize(window, &currentWidth, &currentHeight);

        float currentAspectRatio = static_cast<float>(currentWidth) / currentHeight;
        float targetAspectRatio = 4.0f / 3.0f;

        if (currentAspectRatio < targetAspectRatio)
        {
            int newWidth = (currentHeight * 4) / 3;
            SDL_SetWindowSize(window, newWidth, currentHeight);
        }
    }
}
