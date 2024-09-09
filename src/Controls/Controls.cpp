#include "Controls.hpp"

#include "../Game.hpp"

namespace Controls
{
    SDL_Event event;
    const Uint8* keyboardState = nullptr;
    SDL_GameController* gameController = nullptr;
    std::vector<bool> gameControllerButtonStates{};

    void init()
    {
        gameController = SDL_GameControllerOpen(0);
    }

    void handleEvent(SDL_Event& event)
    {
        switch (event.type) 
        {
        case SDL_KEYDOWN:
            onKeyDown();
            break;
        
        case SDL_KEYUP:
            onKeyUp();
            break;

        case SDL_CONTROLLERDEVICEADDED:
            gameController = SDL_GameControllerOpen(event.cdevice.which);
            break;

        case SDL_CONTROLLERBUTTONDOWN:
            onControllerButtonDown(event.cbutton);
            break;

        case SDL_CONTROLLERBUTTONUP:
            onControllerButtonUp(event.cbutton);
            break;
        }
    }

#pragma region Keyboard
    bool isKeyDown(SDL_Scancode scancode)
    {
        if (keyboardState == nullptr)
            return false;
        return keyboardState[scancode];
    }

    void onKeyDown()
    {
        keyboardState = SDL_GetKeyboardState(nullptr);
    }

    void onKeyUp()
    {
        keyboardState = SDL_GetKeyboardState(nullptr);
    }
#pragma endregion

#pragma region Game Controller
    bool isGameControllerButtonDown(SDL_GameControllerButton button)
    {
        if (gameController == nullptr)
            return false;
        return gameControllerButtonStates[button];
    }

    void onControllerButtonDown(SDL_ControllerButtonEvent& event)
    {
        gameControllerButtonStates[event.button] = true;
    }

    void onControllerButtonUp(SDL_ControllerButtonEvent& event)
    {
        gameControllerButtonStates[event.button] = false;
    }
#pragma endregion
}