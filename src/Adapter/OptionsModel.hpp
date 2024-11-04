#ifndef __SRC_ADAPTER_OPTIONSMODEL_HPP__
#define __SRC_ADAPTER_OPTIONSMODEL_HPP__

#include "../Screen/Options/DefaultOptions.hpp"
#include "../Core/Config.hpp"

struct OptionsModel
{
    float sfxVolumeUnitInterval = DefaultOptions::SFX_VOLUME;
    float musicVolumeUnitInterval = DefaultOptions::MUSIC_VOLUME;
    #ifdef SUPPORTS_AIM_ASSIST
    bool aimAssist = DefaultOptions::AIM_ASSIST;
    #endif
    #ifdef SUPPORTS_CONTROLLER_RUMBLE
    bool controllerRumble = DefaultOptions::CONTROLLER_RUMBLE;
    #endif
    int framerateLimitIndex = DefaultOptions::FRAMERATE_LIMIT_INDEX;
    bool showFramerate = DefaultOptions::SHOW_FRAMERATE;
    #ifdef PLATFORM_GROUP_COMPUTER
    bool fullscreen = DefaultOptions::FULLSCREEN;
    #endif
    int defaultCameraZoomIndex = DefaultOptions::DEFAULT_CAMERA_ZOOM_INDEX;
};

#endif // __SRC_ADAPTER_OPTIONSMODEL_HPP__
