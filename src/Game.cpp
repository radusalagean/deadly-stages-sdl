#include "Game.hpp"

#define GAME_NAME "DeadlyStagesDemo"
#define WIDTH 480
#define HEIGHT 272

namespace Game
{
    bool isRunning = true;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;

    int windowWidth, windowHeight;

    Screen* screen;

    SDL_GameController* gameController;

    void init()
    {
        // Init Logger
        Logger::init();

        // Init SDL
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        window = SDL_CreateWindow(
            GAME_NAME,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_SHOWN
        );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);

        // Init Screen
        screen = new MainMenuScreen();
        screen->init();

        // Init Game Controller
        gameController = SDL_GameControllerOpen(0);
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
                        gameController = SDL_GameControllerOpen(event.cdevice.which);
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
}
