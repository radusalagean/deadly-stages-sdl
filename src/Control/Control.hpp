#ifndef __SRC_CONTROL_CONTROL_HPP__
#define __SRC_CONTROL_CONTROL_HPP__

#include <SDL.h>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>

enum ControlAction
{
    CA_MENUS_UP = 1,
    CA_MENUS_DOWN,
    CA_MENUS_LEFT,
    CA_MENUS_RIGHT,
    CA_MENUS_SELECT,
    CA_MENUS_BACK,

    CA_GAME_MOVE_UP,
    CA_GAME_MOVE_DOWN,
    CA_GAME_MOVE_LEFT,
    CA_GAME_MOVE_RIGHT,
    CA_GAME_FIRE,
    CA_GAME_RELOAD,
    CA_GAME_SPRINT,
    CA_GAME_JUMP,
    CA_GAME_PREVIOUS_WEAPON,
    CA_GAME_NEXT_WEAPON,
    CA_GAME_WEAPON_ID_1,
    CA_GAME_WEAPON_ID_2,
    CA_GAME_WEAPON_ID_3,
    CA_GAME_PAUSE,
    CA_GAME_ZOOM,
    #ifdef SUPPORTS_AUTOAIM
    CA_GAME_AUTOAIM_RIGHT,
    CA_GAME_AUTOAIM_LEFT
    #endif
};

class Control
{
private:
    bool locked = false;

    SDL_GameController* gameController = nullptr;

    std::set<int> pressedActions{};
    std::set<int> blockedActions{};
    std::unordered_map<SDL_Scancode, std::set<int>> keyboardMap{
        {SDL_SCANCODE_UP, {CA_MENUS_UP}},
        {SDL_SCANCODE_DOWN, {CA_MENUS_DOWN}},
        {SDL_SCANCODE_LEFT, {CA_MENUS_LEFT}},
        {SDL_SCANCODE_RIGHT, {CA_MENUS_RIGHT}},
        {SDL_SCANCODE_W, {CA_MENUS_UP, CA_GAME_MOVE_UP}},
        {SDL_SCANCODE_S, {CA_MENUS_DOWN, CA_GAME_MOVE_DOWN}},
        {SDL_SCANCODE_A, {CA_MENUS_LEFT, CA_GAME_MOVE_LEFT}},
        {SDL_SCANCODE_D, {CA_MENUS_RIGHT, CA_GAME_MOVE_RIGHT}},
        {SDL_SCANCODE_RETURN, {CA_MENUS_SELECT}},
        {SDL_SCANCODE_ESCAPE, {CA_MENUS_BACK}},
        {SDL_SCANCODE_LSHIFT, {CA_GAME_SPRINT}},
        {SDL_SCANCODE_SPACE, {CA_GAME_JUMP}},
        {SDL_SCANCODE_R, {CA_GAME_RELOAD}},
        {SDL_SCANCODE_1, {CA_GAME_WEAPON_ID_1}},
        {SDL_SCANCODE_2, {CA_GAME_WEAPON_ID_2}},
        {SDL_SCANCODE_3, {CA_GAME_WEAPON_ID_3}},
    }; // key: scancode, value: actions

    std::unordered_map<int, std::set<int>> mouseMap{
        {SDL_BUTTON_LEFT, {CA_GAME_FIRE}},
        {SDL_BUTTON_RIGHT, {CA_GAME_ZOOM}}
    }; // key: button, value: actions

    std::unordered_map<SDL_GameControllerButton, std::set<int>> gameControllerMap{
        {SDL_CONTROLLER_BUTTON_DPAD_UP, {CA_MENUS_UP, CA_GAME_PREVIOUS_WEAPON}},
        {SDL_CONTROLLER_BUTTON_DPAD_DOWN, {CA_MENUS_DOWN, CA_GAME_NEXT_WEAPON}},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, {CA_MENUS_LEFT}},
        {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, {CA_MENUS_RIGHT}},
        {SDL_CONTROLLER_BUTTON_A, {CA_MENUS_SELECT, CA_GAME_JUMP}},
        {SDL_CONTROLLER_BUTTON_B, {CA_MENUS_BACK, CA_GAME_FIRE}},
        {SDL_CONTROLLER_BUTTON_X, {CA_GAME_RELOAD}},
        {SDL_CONTROLLER_BUTTON_Y, {CA_GAME_SPRINT}},
        {SDL_CONTROLLER_BUTTON_BACK, {CA_GAME_ZOOM}},
        {SDL_CONTROLLER_BUTTON_START, {CA_GAME_PAUSE}},
        #ifdef SUPPORTS_AUTOAIM
        {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, {CA_GAME_AUTOAIM_RIGHT}},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, {CA_GAME_AUTOAIM_LEFT}}
        #endif
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
