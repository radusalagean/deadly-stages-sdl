#include "Game.hpp"

#include "Controls/Controls.hpp"
#include "ScreenManager/ScreenManager.hpp"
#include <SDL_image.h>

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

    void init()
    {
        // Init Logger
        Logger::init();

        // Init Controls
        Controls::init();

        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        IMG_Init(IMG_INIT_PNG);
        window = SDL_CreateWindow(
            GAME_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_RESIZABLE
        );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        SDL_GetRendererOutputSize(renderer, &width, &height);
        logd("Renderer size on init: %dx%d", width, height);

        ScreenManager::getInstance().init();
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
    }

    void update()
    {
        ScreenManager::getInstance().update();
    }   

    void render()
    {
        ScreenManager::getInstance().render();
    }

    void dispose()
    {
        ScreenManager::getInstance().dispose();
    }

    void syncRendererOutputSize()
    {
        SDL_GetRendererOutputSize(renderer, &width, &height);
    }
}
