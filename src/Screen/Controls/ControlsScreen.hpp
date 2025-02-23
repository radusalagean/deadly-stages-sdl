#ifndef __SRC_SCREEN_CONTROLS_CONTROLSSCREEN_HPP__
#define __SRC_SCREEN_CONTROLS_CONTROLSSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Drawable/ImageDrawable.hpp"
#include "../../Core/Config.hpp"

class ControlsScreen : public Screen
{
private:

    #pragma region IDs
    enum class ControlTextureId
    {
        #ifdef PLATFORM_GROUP_COMPUTER
        KEYBOARD,
        MOUSE,
        XBOX_CONTROLLER,
        STICKS,
        #elif defined(PLATFORM_PSP)
        SONY_PSP
        #endif
    };

    enum class ControlPageId
    {
        #ifdef PLATFORM_GROUP_COMPUTER
        KEYBOARD_AND_MOUSE,
        CONTROLLER,
        #elif defined(PLATFORM_PSP)
        SONY_PSP,
        #endif
    };
    #pragma endregion

    #pragma region Configs
    struct ControlSpriteSheetItem
    {
        ControlTextureId textureId;
        int rowIndex;
        int columnIndex;
    };

    struct ControlEntry
    {
        std::vector<ControlSpriteSheetItem> spriteSheetItems;
        std::string label;
    };
    

    struct ControlPage
    {
        ControlPageId id;
        std::string title;
        std::vector<ControlEntry> entries;
    };
    #pragma endregion

    #pragma region Drawables
    struct ControlEntryDrawable
    {
        std::vector<ImageDrawable*> iconDrawables;
        TextDrawable* actionTextDrawable;
    };

    struct ControlPageDrawable
    {
        TextDrawable* titleTextDrawable;
        SDL_Rect leftArrowDstRect;
        SDL_Rect rightArrowDstRect;
        std::vector<ControlEntryDrawable> controlEntryDrawables{};
    };
    #pragma endregion

    static constexpr const int GRID_CELL_SIZE_PX = 16;

    #ifdef PLATFORM_GROUP_COMPUTER
    SDL_Texture* keyboardTexture = nullptr;
    SDL_Texture* mouseTexture = nullptr;
    SDL_Texture* xboxControllerTexture = nullptr;
    SDL_Texture* sticksTexture = nullptr;
    #elif defined(PLATFORM_PSP)
    SDL_Texture* pspTexture = nullptr;
    #endif

    std::vector<ControlPage> controlPages{
        #ifdef PLATFORM_GROUP_COMPUTER
        {
            ControlPageId::KEYBOARD_AND_MOUSE,
            "Keyboard & Mouse", 
            {
                {
                    {
                        {ControlTextureId::KEYBOARD, 22, 0}, // W
                        {ControlTextureId::KEYBOARD, 0, 0}, // A
                        {ControlTextureId::KEYBOARD, 18, 0}, // S
                        {ControlTextureId::KEYBOARD, 3, 0} // D
                    },
                    "Move"
                },
                {
                    {{ControlTextureId::MOUSE, 1, 0}}, // Mouse
                    "Aim"
                },
                {
                    {{ControlTextureId::MOUSE, 1, 1}}, // Mouse Left Click
                    "Fire"
                },
                {
                    {{ControlTextureId::MOUSE, 1, 2}}, // Mouse Right Click
                    "Camera Zoom"
                },
                {
                    {{ControlTextureId::MOUSE, 1, 3}}, // Mouse Wheel Scroll
                    "Cycle Weapons"
                },
                {
                    {
                        {ControlTextureId::KEYBOARD, 12, 4}, // 1
                        {ControlTextureId::KEYBOARD, 13, 4}, // 2
                        {ControlTextureId::KEYBOARD, 14, 4}, // 3
                    },
                    "Select Weapon"
                },
                {
                    {{ControlTextureId::KEYBOARD, 17, 0}}, // R
                    "Reload"
                },
                {
                    {{ControlTextureId::KEYBOARD, 5, 8}}, // Shift
                    "Sprint"
                },
                {
                    {{ControlTextureId::KEYBOARD, 8, 8}}, // Space
                    "Jump"
                },
                {
                    {{ControlTextureId::KEYBOARD, 0, 8}}, // Escape
                    "Pause"
                },
                {
                    {
                        {ControlTextureId::KEYBOARD, 22, 0}, // W
                        {ControlTextureId::KEYBOARD, 0, 0}, // A
                        {ControlTextureId::KEYBOARD, 18, 0}, // S
                        {ControlTextureId::KEYBOARD, 3, 0}, // D
                        {ControlTextureId::KEYBOARD, 12, 12}, // Arrow Up
                        {ControlTextureId::KEYBOARD, 13, 12}, // Arrow Left
                        {ControlTextureId::KEYBOARD, 14, 12}, // Arrow Down
                        {ControlTextureId::KEYBOARD, 15, 12}, // Arrow Right
                    },
                    "Menu - Navigation"
                },
                {
                    {{ControlTextureId::KEYBOARD, 25, 8}}, // Enter
                    "Menu - Confirm"
                },
                {
                    {{ControlTextureId::KEYBOARD, 0, 8}}, // Escape
                    "Menu - Back"
                }
            },
        },
        {
            ControlPageId::CONTROLLER,
            "Controller",
            {
                {
                    {{ControlTextureId::STICKS, 3, 0}}, // Left Stick
                    "Move"
                },
                {
                    {{ControlTextureId::STICKS, 7, 0}}, // Right Stick
                    "Aim"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 25, 0}}, // Right Trigger
                    "Fire"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 19, 0}}, // Left Trigger
                    "Sprint"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 4, 0}}, // A
                    "Jump"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 2, 0}}, // X
                    "Reload"
                },
                {
                    {
                        {ControlTextureId::XBOX_CONTROLLER, 8, 2}, // DPAD Up
                        {ControlTextureId::XBOX_CONTROLLER, 16, 0}, // Left Bumper
                    },
                    "Cycle Weapons - Previous"
                },
                {
                    {
                        {ControlTextureId::XBOX_CONTROLLER, 8, 4}, // DPAD Down
                        {ControlTextureId::XBOX_CONTROLLER, 22, 0}, // Right Bumper
                    },
                    "Cycle Weapons - Next"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 13, 0}}, // Back Button
                    "Camera Zoom"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 11, 0}}, // Start Button
                    "Pause"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 8, 1}}, // DPAD
                    "Menu - Navigation"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 4, 0}}, // A
                    "Menu - Confirm"
                },
                {
                    {{ControlTextureId::XBOX_CONTROLLER, 6, 0}}, // B
                    "Menu - Back"
                }
            }
        },
        #elif defined(PLATFORM_PSP)
        {
            ControlPageId::SONY_PSP,
            "",
            {
                {
                    {{ControlTextureId::SONY_PSP, 0, 0}}, // Analog Stick
                    "Move"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 1}}, // Cross
                    "Jump"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 2}}, // Circle
                    "Fire"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 3}}, // Square
                    "Reload"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 4}}, // Triangle
                    "Sprint"
                },
                {
                    {
                        {ControlTextureId::SONY_PSP, 0, 5}, // DPAD Up
                        {ControlTextureId::SONY_PSP, 0, 6}, // DPAD Down
                    }, 
                    "Cycle Weapons"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 7}}, // Select
                    "(SELECT) Camera Zoom"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 7}}, // Start
                    "(START) Pause"
                },
                {
                    {
                        {ControlTextureId::SONY_PSP, 0, 8}, // L1
                        {ControlTextureId::SONY_PSP, 0, 9}, // R1
                    },
                    "Aim"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 10}}, // DPAD
                    "Menu - Navigation"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 1}}, // Cross
                    "Menu - Confirm"
                },
                {
                    {{ControlTextureId::SONY_PSP, 0, 2}}, // Circle
                    "Menu - Back"
                }
            }
        }
        #endif
    };

    std::vector<ControlPageDrawable> controlPageDrawables{};
    int currentControlPageIndex = 0;

    SDL_Texture* getTextureFromControlTextureId(ControlTextureId textureId);

    void cycleControlPage(int delta);

    int arrowThickness = 0;

public:
    ControlsScreen();
    ~ControlsScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_CONTROLS_CONTROLSSCREEN_HPP__
