#include "Controls.hpp"

#include "../Game.hpp"

namespace Controls
{
    SDL_Event event;
    SDL_GameController* gameController = nullptr;

    void init()
    {
        gameController = SDL_GameControllerOpen(0);
    }

    void handleEvent(SDL_Event& event)
    {
        switch (event.type) 
        {
            case SDL_SCANCODE_ESCAPE:
                Game::isRunning = false;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                gameController = SDL_GameControllerOpen(event.cdevice.which);
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                if(event.cbutton.button == SDL_CONTROLLER_BUTTON_START)
                    Game::isRunning = false;
                break;
        }
    }
}