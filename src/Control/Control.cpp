#include "Control.hpp"

#include "../Game.hpp"
#include <SDL.h>

void Control::init()
{
    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        initGameController(i);
    }
    #ifdef PLATFORM_GROUP_COMPUTER
    currentControlSource = CS_KEYBOARD_AND_MOUSE;
    #else
    currentControlSource = CS_CONTROLLER;
    #endif
}

void Control::initGameController(int index)
{
    if (SDL_IsGameController(index))
    {
        logd("Game controller at index %d is supported", index);
        SDL_GameController* gameController = SDL_GameControllerOpen(index);
        SDL_JoystickID instanceId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
        gameControllers[instanceId] = gameController;
        logd("Game controller with instance id %d initialized", instanceId);
        if (SDL_GameControllerHasRumble(gameController))
            logd("Game controller with instance id %d has rumble", instanceId);
        else
            logd("Game controller with instance id %d does not have rumble", instanceId);
        if (SDL_GameControllerHasRumbleTriggers(gameController))
            logd("Game controller with instance id %d has rumble triggers", instanceId);
        else
            logd("Game controller with instance id %d does not have rumble triggers", instanceId);
    }
    else
    {
        logd("Game controller at index %d is not supported", index);
        logSDLe();
    }
}

void Control::disposeGameController(SDL_JoystickID instanceId, bool removeFromMap)
{
    SDL_GameControllerClose(gameControllers[instanceId]);
    logd("Game controller with instance id %d disposed", instanceId);
    if (removeFromMap)
        gameControllers.erase(instanceId);
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

    case SDL_MOUSEWHEEL:
        onMouseWheelScroll(event.wheel.y);
        break;

    case SDL_CONTROLLERDEVICEADDED:
        initGameController(event.cdevice.which);
        break;

    case SDL_CONTROLLERDEVICEREMOVED:
        disposeGameController(event.cdevice.which);
        break;

    case SDL_CONTROLLERBUTTONDOWN:
        currentGameController = gameControllers[event.cbutton.which];
        onControllerButtonDown(static_cast<SDL_GameControllerButton>(event.cbutton.button));
        break;

    case SDL_CONTROLLERBUTTONUP:
        onControllerButtonUp(static_cast<SDL_GameControllerButton>(event.cbutton.button));
        break;

    case SDL_CONTROLLERAXISMOTION:
        onControllerAxisMotion(event.caxis, event.caxis.which);
        break;
    }
    unlockIfReleased();
}

void Control::dispose()
{
    for (auto& gameController : gameControllers)
    {
        disposeGameController(gameController.first, false);
    }
    gameControllers.clear();
}

#pragma region Actions
void Control::onActionDown(const PressedActionData data)
{
    pressedActions.insert(data);
}

void Control::onActionUp(const PressedActionData data)
{
    releaseAction(data);
}

bool Control::isActionDown(int action, const PressedActionData** pressedActionDataOut)
{
    if (locked)
        return false;
    for (auto& data : pressedActions)
    {
        if (data.action == action)
        {
            if (pressedActionDataOut != nullptr)
                *pressedActionDataOut = &data;
            return true;
        }
    }
    return false;
}

bool Control::isAnyActionDown(const std::vector<int>& actions)
{
    if (locked)
        return false;
    for (const auto& action : actions) {
        if (isActionDown(action, nullptr)) {
            return true;
        }
    }
    return false;
}

bool Control::isActionUp(int action)
{
    return !isActionDown(action, nullptr);
}

void Control::releaseAction(const PressedActionData data)
{
    pressedActions.erase(data);
}

void Control::releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(const PressedActionData data)
{
    switch (data.trigger)
    {
    case TRIGGER_KEYBOARD:
        for (auto& associatedAction : keyboardMap[static_cast<SDL_Scancode>(data.pressedActionTriggerId)])
            releaseAction({associatedAction, data.trigger, data.pressedActionTriggerId});
        blockedKeys.insert(data.pressedActionTriggerId);
        break;
    case TRIGGER_MOUSE_BUTTON:
        for (auto& associatedAction : mouseMap[data.pressedActionTriggerId])
            releaseAction({associatedAction, data.trigger, data.pressedActionTriggerId});
        blockedMouseButtons.insert(data.pressedActionTriggerId);
        break;
    case TRIGGER_MOUSE_WHEEL_MOTION:
        for (auto& associatedAction : mouseWheelMotionMap[data.pressedActionTriggerId])
            releaseAction({associatedAction, data.trigger, data.pressedActionTriggerId});
        blockedMouseWheelMotion = false; // Mouse wheel motion is blocked when the action is triggered, and unblocked here
        break;
    case TRIGGER_CONTROLLER_BUTTON:
        for (auto& associatedAction : gameControllerMap[static_cast<SDL_GameControllerButton>(data.pressedActionTriggerId)])
            releaseAction({associatedAction, data.trigger, data.pressedActionTriggerId});
        blockedControllerButtons.insert(data.pressedActionTriggerId);
        break;
    case TRIGGER_CONTROLLER_AXIS_MOTION:
        for (auto& associatedAction : controllerAxisMotionMap[data.pressedActionTriggerId])
            releaseAction({associatedAction, data.trigger, data.pressedActionTriggerId});
        blockedControllerAxisMotions.insert(data.pressedActionTriggerId);
        break;
    }
}

void Control::unblockActionTrigger(const PressedActionData data)
{
    switch (data.trigger)
    {
    case TRIGGER_KEYBOARD:
        blockedKeys.erase(data.pressedActionTriggerId);
        break;
    case TRIGGER_MOUSE_BUTTON:
        blockedMouseButtons.erase(data.pressedActionTriggerId);
        break;
    case TRIGGER_MOUSE_WHEEL_MOTION:
        blockedMouseWheelMotion = false;
        break;
    case TRIGGER_CONTROLLER_BUTTON:
        blockedControllerButtons.erase(data.pressedActionTriggerId);
        break;
    case TRIGGER_CONTROLLER_AXIS_MOTION:
        blockedControllerAxisMotions.erase(data.pressedActionTriggerId);
        break;
    }
}

void Control::performAndScheduleActionTriggerRelease(const PressedActionData data, Uint32 delayMs)
{
    if (isActionDown(data.action, nullptr))
        return;
        
    onActionDown(data);
    
    ScheduleTimerData* timerData = new ScheduleTimerData{data, this};

    SDL_AddTimer(delayMs, [](Uint32 interval, void* param) -> Uint32 {
        ScheduleTimerData* timerData = static_cast<ScheduleTimerData*>(param);
        timerData->control->releaseAction(timerData->data);
        timerData->control->unblockActionTrigger(timerData->data);
        delete timerData;
        return 0;  // Don't repeat the timer
    }, timerData);
}
#pragma endregion

#pragma region Keyboard
void Control::onKeyDown(SDL_Scancode scancode)
{
    currentControlSource = CS_KEYBOARD_AND_MOUSE;
    if (blockedKeys.find(scancode) != blockedKeys.end())
        return;
    for (auto& action : keyboardMap[scancode])
        onActionDown({action, TRIGGER_KEYBOARD, scancode});
}

void Control::onKeyUp(SDL_Scancode scancode)
{
    blockedKeys.erase(scancode);
    for (auto& action : keyboardMap[scancode])
        onActionUp({action, TRIGGER_KEYBOARD, scancode});
}   
#pragma endregion

#pragma region Mouse
void Control::onMouseButtonDown(Uint8 button)
{
    currentControlSource = CS_KEYBOARD_AND_MOUSE;
    if (blockedMouseButtons.find(button) != blockedMouseButtons.end())
        return;
    for (auto& action : mouseMap[button])
        onActionDown({action, TRIGGER_MOUSE_BUTTON, button});
}

void Control::onMouseButtonUp(Uint8 button)
{
    blockedMouseButtons.erase(button);
    for (auto& action : mouseMap[button])
        onActionUp({action, TRIGGER_MOUSE_BUTTON, button});
}

void Control::onMouseWheelScroll(Sint32 y)
{
    currentControlSource = CS_KEYBOARD_AND_MOUSE;
    if (y == 0)
        blockedMouseWheelMotion = false;
    if (blockedMouseWheelMotion)
        return;
    int triggerId = 0;
    if (y > 0) 
        triggerId = 1;
    else if (y < 0) 
        triggerId = -1;
    for (auto& action : mouseWheelMotionMap[triggerId])
        performAndScheduleActionTriggerRelease({action, TRIGGER_MOUSE_WHEEL_MOTION, triggerId});
    blockedMouseWheelMotion = true;
}
#pragma endregion

#pragma region Game Controller
void Control::onControllerButtonDown(SDL_GameControllerButton button)
{
    currentControlSource = CS_CONTROLLER;
    if (blockedControllerButtons.find(button) != blockedControllerButtons.end())
        return;
    for (auto& action : gameControllerMap[button])
        onActionDown({action, TRIGGER_CONTROLLER_BUTTON, button});
}

void Control::onControllerButtonUp(SDL_GameControllerButton button)
{
    blockedControllerButtons.erase(button);
    for (auto& action : gameControllerMap[button])
        onActionUp({action, TRIGGER_CONTROLLER_BUTTON, button});
}

void Control::onControllerAxisMotion(const SDL_ControllerAxisEvent& axisEvent, SDL_JoystickID instanceId)
{
    // Normalize the axis value to a range of -1.0 to 1.0
    float value = static_cast<float>(axisEvent.value) / static_cast<float>(ANALOG_MAX_VALUE);

    if (std::abs(value) < static_cast<float>(ANALOG_DEADZONE) / static_cast<float>(ANALOG_MAX_VALUE))
    {
        value = 0.0f;
    }
    if (value == 0.0f)
    {
        blockedControllerAxisMotions.erase(axisEvent.axis);
    }
    else
    {
        currentControlSource = CS_CONTROLLER;
        currentGameController = gameControllers[instanceId];
    }
    if (blockedControllerAxisMotions.find(axisEvent.axis) != blockedControllerAxisMotions.end())
        return;
    for (auto& action : controllerAxisMotionMap[axisEvent.axis])
    {
        if (std::abs(value) > 0.0f)
            onActionDown({action, TRIGGER_CONTROLLER_AXIS_MOTION, axisEvent.axis});
        else
            onActionUp({action, TRIGGER_CONTROLLER_AXIS_MOTION, axisEvent.axis});
    }
    switch (axisEvent.axis)
    {
    case SDL_CONTROLLER_AXIS_LEFTX:
        leftStickValue.x = value;
        break;
    case SDL_CONTROLLER_AXIS_LEFTY:
        leftStickValue.y = value;
        break;
    case SDL_CONTROLLER_AXIS_RIGHTX:
        rightStickValue.x = value;
        break;
    case SDL_CONTROLLER_AXIS_RIGHTY:
        rightStickValue.y = value;
        break;
    case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        leftTriggerValue = value;
        break;
    case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
        rightTriggerValue = value;
        break;
    }
}

void Control::rumbleCurrentControllerIfActive(ControllerRumbleConfig config)
{
    if (currentGameController == nullptr || currentControlSource != CS_CONTROLLER)
        return;
    ControllerRumbleTrigger trigger = config.trigger;
    if (trigger == RUMBLE_TRIGGER && !SDL_GameControllerHasRumbleTriggers(currentGameController))
        trigger = RUMBLE_DEFAULT;
    switch (trigger)
    {
    case RUMBLE_DEFAULT:
        SDL_GameControllerRumble(currentGameController, 0xFFFF * config.lowFrequencyRumbleIntensity, 
            0xFFFF * config.highFrequencyRumbleIntensity, config.durationMs);
        break;
    case RUMBLE_TRIGGER:
        SDL_GameControllerRumbleTriggers(currentGameController, 0xFFFF * config.lowFrequencyRumbleIntensity, 
            0xFFFF * config.highFrequencyRumbleIntensity, config.durationMs);
        break;
    }
}
#pragma endregion