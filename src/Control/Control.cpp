#include "Control.hpp"

#include "../Game.hpp"


void Control::init()
{
    gameController = SDL_GameControllerOpen(0);
}

void Control::handleEvent(SDL_Event& event)
{
    switch (event.type) 
    {
    case SDL_KEYDOWN:
        onKeyDown(event.key.keysym.scancode);
        break;
    
    case SDL_KEYUP:
        onKeyUp(event.key.keysym.scancode);
        break;

    case SDL_MOUSEBUTTONDOWN:
        onMouseButtonDown(event.button.button);
        break;

    case SDL_MOUSEBUTTONUP:
        onMouseButtonUp(event.button.button);
        break;

    case SDL_CONTROLLERDEVICEADDED:
        gameController = SDL_GameControllerOpen(event.cdevice.which);
        break;

    case SDL_CONTROLLERBUTTONDOWN:
        onControllerButtonDown(static_cast<SDL_GameControllerButton>(event.cbutton.button));
        break;

    case SDL_CONTROLLERBUTTONUP:
        onControllerButtonUp(static_cast<SDL_GameControllerButton>(event.cbutton.button));
        break;
    }
    unlockIfReleased();
}

#pragma region Actions
bool Control::isActionDown(int action)
{
    if (locked)
        return false;
    return std::find(pressedActions.begin(), pressedActions.end(), action) != pressedActions.end();
}

bool Control::isActionUp(int action)
{
    return std::find(pressedActions.begin(), pressedActions.end(), action) == pressedActions.end();
}
#pragma endregion

#pragma region Keyboard
void Control::onKeyDown(SDL_Scancode scancode)
{
    for (auto& action : keyboardMap[scancode])
        pressedActions.insert(action);
}

void Control::onKeyUp(SDL_Scancode scancode)
{
    for (auto& action : keyboardMap[scancode])

        pressedActions.erase(action);
}   
#pragma endregion

#pragma region Mouse
void Control::onMouseButtonDown(Uint8 button)
{
    for (auto& action : mouseMap[button])
        pressedActions.insert(action);
}

void Control::onMouseButtonUp(Uint8 button)
{
    for (auto& action : mouseMap[button])
        pressedActions.erase(action);
}
#pragma endregion

#pragma region Game Controller
void Control::onControllerButtonDown(SDL_GameControllerButton button)
{
    for (auto& action : gameControllerMap[button])
        pressedActions.insert(action);
}

void Control::onControllerButtonUp(SDL_GameControllerButton button)
{
    for (auto& action : gameControllerMap[button])
        pressedActions.erase(action);
}
#pragma endregion
