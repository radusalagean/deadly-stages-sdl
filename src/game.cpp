#include "game.hpp"

namespace Game
{
    bool isRunning = true;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    Screen* screen;

    void init()
    {
        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        window = SDL_CreateWindow(
            "window",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            480,
            272,
            0
        );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        // Init Screen
        screen = new MainMenuScreen(renderer);
        screen->init();
    }

    void loop()
    {
        while (isRunning)
        {
            // SDL Event Loop
            if (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        isRunning = 0;
                        break;
                    case SDL_CONTROLLERDEVICEADDED:
                        SDL_GameControllerOpen(event.cdevice.which);
                        break;
                    case SDL_CONTROLLERBUTTONDOWN:
                        if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                            isRunning = 0;
                        break;
                }
            }

            // Update Screen
            if (screen != NULL)
                if (screen->update())
                    screen->render();
        }
    }

    void dispose()
    {

    }
} // namespace Game
