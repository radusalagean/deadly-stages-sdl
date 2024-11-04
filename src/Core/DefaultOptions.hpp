#ifndef __SRC_CORE_DEFAULTOPTIONS_HPP__
#define __SRC_CORE_DEFAULTOPTIONS_HPP__

#include "Config.hpp"

struct DefaultOptions
{
    static constexpr float SFX_VOLUME = 0.8f;
    static constexpr float MUSIC_VOLUME = 0.8f;
    #ifdef SUPPORTS_AIM_ASSIST
    static constexpr bool AIM_ASSIST = true;
    #endif
    #ifdef SUPPORTS_CONTROLLER_RUMBLE
    static constexpr bool CONTROLLER_RUMBLE = true;
    #endif
    static constexpr int FRAMERATE_LIMIT_INDEX = 1;
    static constexpr bool SHOW_FRAMERATE = false;
    #ifdef PLATFORM_GROUP_COMPUTER
    static constexpr bool FULLSCREEN = true;
    #endif
    #ifdef PLATFORM_PSP
    static constexpr int DEFAULT_CAMERA_ZOOM_INDEX = 1;
    #else
    static constexpr int DEFAULT_CAMERA_ZOOM_INDEX = 2;
    #endif
};

#endif // __SRC_CORE_DEFAULTOPTIONS_HPP__