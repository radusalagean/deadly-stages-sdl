#ifndef __SRC_CONTROLS_CONTROLS_HPP__
#define __SRC_CONTROLS_CONTROLS_HPP__

#include <SDL.h>
#include <vector>
#include <set>

enum ControlAction
{
    CA_UP = 1,
    CA_DOWN,
    CA_LEFT,
    CA_RIGHT,
    CA_SELECT,
    CA_ESCAPE
};

class Controls
{
private:
    bool locked = false;

    SDL_GameController* gameController = nullptr;

    std::set<int> pressedActions{};
    std::unordered_map<SDL_Scancode, std::set<int>> keyboardMap{
        {SDL_SCANCODE_UP, {CA_UP}},
        {SDL_SCANCODE_DOWN, {CA_DOWN}},
        {SDL_SCANCODE_LEFT, {CA_LEFT}},
        {SDL_SCANCODE_RIGHT, {CA_RIGHT}},
        {SDL_SCANCODE_RETURN, {CA_SELECT}},
        {SDL_SCANCODE_ESCAPE, {CA_ESCAPE}}
    }; // key: scancode, value: actions
    std::unordered_map<SDL_GameControllerButton, std::set<int>> gameControllerMap{
        {SDL_CONTROLLER_BUTTON_DPAD_UP, {CA_UP}},
        {SDL_CONTROLLER_BUTTON_DPAD_DOWN, {CA_DOWN}},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, {CA_LEFT}},
        {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, {CA_RIGHT}},
        {SDL_CONTROLLER_BUTTON_A, {CA_SELECT}}
    }; // key: button, value: actions

    inline void unlockIfReleased()
    {
        if (!locked)
            return;
        if (pressedActions.empty())
            locked = false;
    }

public:
    void init();
    void handleEvent(SDL_Event& event);

    // Actions
    bool isActionDown(int action);
    bool isActionUp(int action);

    // Keyboard
    void onKeyDown(SDL_Scancode scancode);
    void onKeyUp(SDL_Scancode scancode);

    // Game Controller
    void onControllerButtonDown(SDL_GameControllerButton button);
    void onControllerButtonUp(SDL_GameControllerButton button);

    inline void lock() { locked = true; }
};

#endif // __SRC_CONTROLS_CONTROLS_HPP__
