#ifndef __SRC_CONTROLS_CONTROLS_HPP__
#define __SRC_CONTROLS_CONTROLS_HPP__

#include <SDL.h>
#include <vector>
namespace Controls
{
    extern SDL_Event event;
    extern const Uint8* keyboardState;
    extern SDL_GameController* gameController;
    extern std::vector<bool> gameControllerButtonStates;

    void init();
    void handleEvent(SDL_Event& event);
    
    // Keyboard
    bool isKeyDown(SDL_Scancode scancode);
    void onKeyDown();
    void onKeyUp();

    // Game Controller
    bool isGameControllerButtonDown(SDL_GameControllerButton button);
    void onControllerButtonDown(SDL_ControllerButtonEvent& event);
    void onControllerButtonUp(SDL_ControllerButtonEvent& event);
}

#endif // __SRC_CONTROLS_CONTROLS_HPP__
