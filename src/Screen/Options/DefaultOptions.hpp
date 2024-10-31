#ifndef __SRC_SCREEN_OPTIONS_DEFAULTOPTIONS_HPP__
#define __SRC_SCREEN_OPTIONS_DEFAULTOPTIONS_HPP__

struct DefaultOptions
{
    static constexpr float SFX_VOLUME = 0.8f;
    static constexpr float MUSIC_VOLUME = 0.8f;
    static constexpr bool AIM_ASSIST = true;
    static constexpr bool CONTROLLER_RUMBLE = true;
    static constexpr int FRAMERATE_LIMIT_INDEX = 1;
    static constexpr bool SHOW_FRAMERATE = false;
    static constexpr int DEFAULT_CAMERA_ZOOM_INDEX = 1;
};

#endif // __SRC_SCREEN_OPTIONS_DEFAULTOPTIONS_HPP__
