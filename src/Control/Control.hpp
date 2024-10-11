#ifndef __SRC_CONTROL_CONTROL_HPP__
#define __SRC_CONTROL_CONTROL_HPP__

#include <SDL.h>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

enum ControlAction
{
    CA_UP = 1,
    CA_DOWN,
    CA_LEFT,
    CA_RIGHT,
    CA_SELECT,
    CA_ESCAPE,
    CA_FIRE,
    CA_RELOAD,
    CA_SPRINT,
    CA_JUMP,
    CA_PREVIOUS_WEAPON,
    CA_NEXT_WEAPON,
    CA_DEBUG_WEAPON_1,
    CA_DEBUG_WEAPON_2,
    CA_DEBUG_WEAPON_3
};

class Control
{
private:
    bool locked = false;

    SDL_GameController* gameController = nullptr;

    std::set<int> pressedActions{};
    std::set<int> blockedActions{};
    std::unordered_map<SDL_Scancode, std::set<int>> keyboardMap{
        {SDL_SCANCODE_UP, {CA_UP}},
        {SDL_SCANCODE_DOWN, {CA_DOWN}},
        {SDL_SCANCODE_LEFT, {CA_LEFT}},
        {SDL_SCANCODE_RIGHT, {CA_RIGHT}},
        {SDL_SCANCODE_W, {CA_UP}},
        {SDL_SCANCODE_S, {CA_DOWN}},
        {SDL_SCANCODE_A, {CA_LEFT}},
        {SDL_SCANCODE_D, {CA_RIGHT}},
        {SDL_SCANCODE_RETURN, {CA_SELECT}},
        {SDL_SCANCODE_ESCAPE, {CA_ESCAPE}},
        {SDL_SCANCODE_LSHIFT, {CA_SPRINT}},
        {SDL_SCANCODE_SPACE, {CA_JUMP}},
        {SDL_SCANCODE_R, {CA_RELOAD}},
        {SDL_SCANCODE_1, {CA_DEBUG_WEAPON_1}},
        {SDL_SCANCODE_2, {CA_DEBUG_WEAPON_2}},
        {SDL_SCANCODE_3, {CA_DEBUG_WEAPON_3}}
    }; // key: scancode, value: actions

    std::unordered_map<int, std::set<int>> mouseMap{
        {SDL_BUTTON_LEFT, {CA_FIRE}}
    }; // key: button, value: actions

    std::unordered_map<SDL_GameControllerButton, std::set<int>> gameControllerMap{
        {SDL_CONTROLLER_BUTTON_DPAD_UP, {CA_UP}},
        {SDL_CONTROLLER_BUTTON_DPAD_DOWN, {CA_DOWN}},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, {CA_LEFT}},
        {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, {CA_RIGHT}},
        {SDL_CONTROLLER_BUTTON_A, {CA_SELECT, CA_SPRINT}},
        {SDL_CONTROLLER_BUTTON_B, {CA_FIRE}},
        {SDL_CONTROLLER_BUTTON_X, {CA_JUMP}},
        {SDL_CONTROLLER_BUTTON_Y, {CA_RELOAD}},
        {SDL_CONTROLLER_BUTTON_START, {CA_ESCAPE}}
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

    // Mouse
    void onMouseButtonDown(Uint8 button);
    void onMouseButtonUp(Uint8 button);
    void onMouseWheelScroll(Sint32 y);

    // Game Controller
    void onControllerButtonDown(SDL_GameControllerButton button);
    void onControllerButtonUp(SDL_GameControllerButton button);

    inline void lock() { locked = true; }

    void onActionDown(int action);
    void onActionUp(int action);
    void releaseAction(int action);
    void releaseAndBlockAction(int action);
    void performAndScheduleActionRelease(int action, Uint32 delayMs = 1000);
};

struct ScheduleTimerData {
    int action;
    Control* control;
};

#endif // __SRC_CONTROL_CONTROL_HPP__
