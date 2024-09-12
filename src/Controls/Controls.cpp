#include "Controls.hpp"

#include "../Game.hpp"


void Controls::init()
{
    gameController = SDL_GameControllerOpen(0);
}

void Controls::handleEvent(SDL_Event& event)
{
    switch (event.type) 
    {
    case SDL_KEYDOWN:
        onKeyDown(event.key.keysym.scancode);
        break;
    
    case SDL_KEYUP:
        onKeyUp(event.key.keysym.scancode);
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
bool Controls::isActionDown(int action)
{
    if (locked)
        return false;
    return std::find(pressedActions.begin(), pressedActions.end(), action) != pressedActions.end();
}

bool Controls::isActionUp(int action)
{
    return std::find(pressedActions.begin(), pressedActions.end(), action) == pressedActions.end();
}
#pragma endregion

#pragma region Keyboard
void Controls::onKeyDown(SDL_Scancode scancode)
{
    for (auto& action : keyboardMap[scancode])
        pressedActions.insert(action);
}

void Controls::onKeyUp(SDL_Scancode scancode)
{
    for (auto& action : keyboardMap[scancode])
        pressedActions.erase(action);
}   
#pragma endregion

#pragma region Game Controller
void Controls::onControllerButtonDown(SDL_GameControllerButton button)
{
    for (auto& action : gameControllerMap[button])
        pressedActions.insert(action);
}

void Controls::onControllerButtonUp(SDL_GameControllerButton button)
{
    for (auto& action : gameControllerMap[button])
        pressedActions.erase(action);
}
#pragma endregion
