#include "Game.hpp"

#include "Controls/Controls.hpp"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Core/FontManager.hpp"
#include "ScreenManager/ScreenManager.hpp"
#include "Core/PrimitiveShapeHelper.hpp"

#define GAME_NAME "DeadlyStagesDemo"
#define WIDTH 480
#define HEIGHT 272

namespace Game
{
    bool isRunning = true;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int width = 0;
    int height = 0;
    SDL_Surface* windowSurface = nullptr;

    FontManager fontManager;
    ScreenManager screenManager;
    PrimitiveShapeHelper primitiveShapeHelper;

    void init()
    {
        // Init Logger
        Logger::init();

        // Init Controls
        Controls::init();

        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        IMG_Init(IMG_INIT_PNG);
        TTF_Init();
        window = SDL_CreateWindow(
            GAME_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_RESIZABLE
        );

        // Set minimum window size
        SDL_SetWindowMinimumSize(window, WIDTH, HEIGHT);

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        SDL_GetRendererOutputSize(renderer, &width, &height);
        logd("Renderer size on init: %dx%d", width, height);

        screenManager.init();
    }

    void loop()
    {
        handleEvents();
        update();
        render();
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
            Controls::handleEvent(event);
        }
        screenManager.handleEvents();
    }

    void update()
    {
        screenManager.update();
    }   

    void render()
    {
        screenManager.render();
    }

    void dispose()
    {
        screenManager.dispose();
        fontManager.dispose();
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
