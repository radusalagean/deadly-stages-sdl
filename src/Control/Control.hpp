#ifndef __SRC_CONTROL_CONTROL_HPP__
#define __SRC_CONTROL_CONTROL_HPP__

#include <SDL.h>
#include <vector>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <tuple>
#include <map>
#include "../Core/Vector2D.hpp"
#include "../Core/Config.hpp"

enum ControlSource
{
    CS_KEYBOARD_AND_MOUSE = 1,
    CS_CONTROLLER
};

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
    CA_GAME_MOVE_ANALOG,
    CA_GAME_AIM_ANALOG,
    CA_GAME_FIRE,
    CA_GAME_RELOAD,
    CA_GAME_SPRINT,
    CA_GAME_JUMP,
    CA_GAME_PREVIOUS_WEAPON,
    CA_GAME_NEXT_WEAPON,
    CA_GAME_WEAPON_SLOT_1,
    CA_GAME_WEAPON_SLOT_2,
    CA_GAME_WEAPON_SLOT_3,
    CA_GAME_PAUSE,
    CA_GAME_ZOOM,
    #ifdef SUPPORTS_AIM_ASSIST
    CA_GAME_AIM_ASSIST_RIGHT,
    CA_GAME_AIM_ASSIST_LEFT
    #endif
};

enum PressedActionTrigger
{
    TRIGGER_KEYBOARD = 1,
    TRIGGER_MOUSE_BUTTON,
    TRIGGER_MOUSE_WHEEL_MOTION,
    TRIGGER_CONTROLLER_BUTTON,
    TRIGGER_CONTROLLER_AXIS_MOTION
};

struct PressedActionData 
{
    int action;
    PressedActionTrigger trigger;
    int pressedActionTriggerId;

    bool operator<(const PressedActionData& other) const {
        return std::tie(action, trigger, pressedActionTriggerId) < std::tie(other.action, other.trigger, other.pressedActionTriggerId);
    }

    bool operator==(const PressedActionData& other) const {
        return action == other.action &&
               trigger == other.trigger &&
               pressedActionTriggerId == other.pressedActionTriggerId;
    }
};

enum ControllerRumbleTrigger
{
    RUMBLE_DEFAULT = 1,
    RUMBLE_TRIGGER
};

struct ControllerRumbleConfig
{
    ControllerRumbleTrigger trigger = RUMBLE_DEFAULT;
    float lowFrequencyRumbleIntensity = 0.0f;
    float highFrequencyRumbleIntensity = 0.0f;
    Uint32 durationMs = 0;
};

class Control
{
private:
    bool locked = false;

    std::map<SDL_JoystickID, SDL_GameController*> gameControllers;
    SDL_GameController* currentGameController = nullptr;

    ControlSource currentControlSource;

    std::set<PressedActionData> pressedActions{};
    std::set<int> blockedKeys{};
    std::set<int> blockedMouseButtons{};
    bool blockedMouseWheelMotion = false;
    std::set<int> blockedControllerButtons{};
    std::set<int> blockedControllerAxisMotions{};

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
        {SDL_SCANCODE_ESCAPE, {CA_MENUS_BACK, CA_GAME_PAUSE}},
        {SDL_SCANCODE_LSHIFT, {CA_GAME_SPRINT}},
        {SDL_SCANCODE_SPACE, {CA_GAME_JUMP}},
        {SDL_SCANCODE_R, {CA_GAME_RELOAD}},
        {SDL_SCANCODE_1, {CA_GAME_WEAPON_SLOT_1}},
        {SDL_SCANCODE_2, {CA_GAME_WEAPON_SLOT_2}},
        {SDL_SCANCODE_3, {CA_GAME_WEAPON_SLOT_3}}
    }; // key: scancode, value: actions

    std::unordered_map<int, std::set<int>> mouseMap{
        {SDL_BUTTON_LEFT, {CA_GAME_FIRE}},
        {SDL_BUTTON_RIGHT, {CA_GAME_ZOOM}}
    }; // key: button, value: actions

    std::unordered_map<int, std::set<int>> mouseWheelMotionMap{
        {1, {CA_GAME_NEXT_WEAPON}},
        {-1, {CA_GAME_PREVIOUS_WEAPON}}
    }; // key: y, value: actions

    #ifdef PLATFORM_PSP
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
        #ifdef SUPPORTS_AIM_ASSIST
        {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, {CA_GAME_AIM_ASSIST_RIGHT}},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, {CA_GAME_AIM_ASSIST_LEFT}}
        #endif
    }; // key: button, value: actions

    std::unordered_map<int, std::set<int>> controllerAxisMotionMap{
        {SDL_CONTROLLER_AXIS_LEFTX, {CA_GAME_MOVE_ANALOG}},
        {SDL_CONTROLLER_AXIS_LEFTY, {CA_GAME_MOVE_ANALOG}}
    }; // key: axis, value: actions
    #else
    std::unordered_map<SDL_GameControllerButton, std::set<int>> gameControllerMap{
        {SDL_CONTROLLER_BUTTON_DPAD_UP, {CA_MENUS_UP, CA_GAME_PREVIOUS_WEAPON}},
        {SDL_CONTROLLER_BUTTON_DPAD_DOWN, {CA_MENUS_DOWN, CA_GAME_NEXT_WEAPON}},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, {CA_MENUS_LEFT}},
        {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, {CA_MENUS_RIGHT}},
        {SDL_CONTROLLER_BUTTON_A, {CA_MENUS_SELECT, CA_GAME_JUMP}},
        {SDL_CONTROLLER_BUTTON_B, {CA_MENUS_BACK}},
        {SDL_CONTROLLER_BUTTON_X, {CA_GAME_RELOAD}},
        {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, {CA_GAME_NEXT_WEAPON}},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, {CA_GAME_PREVIOUS_WEAPON}},
        {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, {CA_GAME_FIRE}},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, {CA_GAME_SPRINT}},
        {SDL_CONTROLLER_BUTTON_BACK, {CA_GAME_ZOOM}},
        {SDL_CONTROLLER_BUTTON_START, {CA_GAME_PAUSE}}
    }; // key: button, value: actions

    std::unordered_map<int, std::set<int>> controllerAxisMotionMap{
        {SDL_CONTROLLER_AXIS_LEFTX, {CA_GAME_MOVE_ANALOG}},
        {SDL_CONTROLLER_AXIS_LEFTY, {CA_GAME_MOVE_ANALOG}},
        {SDL_CONTROLLER_AXIS_RIGHTX, {CA_GAME_AIM_ANALOG}},
        {SDL_CONTROLLER_AXIS_RIGHTY, {CA_GAME_AIM_ANALOG}},
        {SDL_CONTROLLER_AXIS_TRIGGERLEFT, {CA_GAME_SPRINT}},
        {SDL_CONTROLLER_AXIS_TRIGGERRIGHT, {CA_GAME_FIRE}}
    }; // key: axis, value: actions
    #endif

    static constexpr int ANALOG_DEADZONE = 8000;
    static constexpr int ANALOG_MAX_VALUE = 32767;

    Vector2D leftStickValue;
    Vector2D rightStickValue;
    float leftTriggerValue = 0.0f;
    float rightTriggerValue = 0.0f;

    inline void unlockIfReleased()
    {
        if (!locked)
            return;
        if (pressedActions.empty())
            locked = false;
    }

public:
    void init();
    void initGameController(int index);
    void disposeGameController(SDL_JoystickID instanceId, bool removeFromMap = true);
    void handleEvent(SDL_Event& event);
    void dispose();

    // Actions
    void onActionDown(const PressedActionData data);
    void onActionUp(const PressedActionData data);
    bool isActionDown(int action, const PressedActionData** pressedActionDataOut = nullptr);
    bool isAnyActionDown(const std::vector<int>& actions);
    bool isActionUp(int action);
    void releaseAction(const PressedActionData data);
    void releaseAssociatedActionsAndBlockActionTrigger(const PressedActionData data);
    void unblockActionTrigger(const PressedActionData data);
    void performAndScheduleActionTriggerRelease(const PressedActionData data, Uint32 delayMs = 1000);

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
    void onControllerAxisMotion(const SDL_ControllerAxisEvent& axisEvent, SDL_JoystickID instanceId);
    void rumbleCurrentControllerIfActive(ControllerRumbleConfig config);

    inline void lock() { locked = true; }
    
    const Vector2D& getLeftStickValue() const { return leftStickValue; }
    const Vector2D& getRightStickValue() const { return rightStickValue; }
    float getLeftTriggerValue() const { return leftTriggerValue; }
    float getRightTriggerValue() const { return rightTriggerValue; }

    const ControlSource getCurrentControlSource() const { return currentControlSource; }
};

struct ScheduleTimerData {
    PressedActionData data;
    Control* control;
};

#endif // __SRC_CONTROL_CONTROL_HPP__
