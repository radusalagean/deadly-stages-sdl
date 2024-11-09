#include "OptionsAdapter.hpp"

#include "../Game.hpp"
#include "../Core/AudioManager.hpp"
#include "../Core/MathUtils.hpp"
#include "../Level/Camera.hpp"
#include "../Core/Config.hpp"
#include <tinyxml2.h>

using namespace tinyxml2;

namespace OptionsAdapter
{

    static constexpr const char* ROOT_OPTIONS = "options";
    static constexpr const char* ROOT_OPTIONS_ATTR_SCHEMA_VERSION = "schema_version";
    static constexpr const int CURRENT_SCHEMA_VERSION = 1;

    static constexpr const char* OPTION_SFX_VOLUME_PERCENT = "sfx_volume_percent";
    static constexpr const char* OPTION_MUSIC_VOLUME_PERCENT = "music_volume_percent";
    #ifdef SUPPORTS_AIM_ASSIST
    static constexpr const char* OPTION_AIM_ASSIST = "aim_assist";
    #endif
    #ifdef SUPPORTS_CONTROLLER_RUMBLE
    static constexpr const char* OPTION_CONTROLLER_RUMBLE = "controller_rumble";
    #endif
    static constexpr const char* OPTION_FRAMERATE_LIMIT_INDEX = "framerate_limit_index";
    static constexpr const char* OPTION_SHOW_FRAMERATE = "show_framerate";
    #ifdef PLATFORM_GROUP_COMPUTER
    static constexpr const char* OPTION_FULLSCREEN = "fullscreen";
    #endif
    static constexpr const char* OPTION_DEFAULT_CAMERA_ZOOM_INDEX = "default_camera_zoom_index";

    OptionsModel extractOptionsFromGame()
    {
        OptionsModel options;

        options.sfxVolumeUnitInterval = Game::audioManager.getSoundVolumeUnitInterval();

        options.musicVolumeUnitInterval = Game::audioManager.getMusicVolumeUnitInterval();

        #ifdef SUPPORTS_AIM_ASSIST
        options.aimAssist = Game::control.aimAssist;
        #endif

        #ifdef SUPPORTS_CONTROLLER_RUMBLE
        options.controllerRumble = Game::control.controllerRumble;
        #endif

        options.framerateLimitIndex = Game::getFramerateLimitIndex();

        options.showFramerate = Game::showFramerate;

        #ifdef PLATFORM_GROUP_COMPUTER
        options.fullscreen = Game::fullscreen;
        #endif

        options.defaultCameraZoomIndex = Camera::defaultZoomIndex;

        return options;
    }

    void applyOptionsFromModel(OptionsModel& options)
    {
        Game::audioManager.setSoundVolume(options.sfxVolumeUnitInterval);

        Game::audioManager.setMusicVolume(options.musicVolumeUnitInterval);

        #ifdef SUPPORTS_AIM_ASSIST
        Game::control.aimAssist = options.aimAssist;
        #endif

        #ifdef SUPPORTS_CONTROLLER_RUMBLE
        Game::control.controllerRumble = options.controllerRumble;
        #endif

        Game::setFramerateLimitIndex(options.framerateLimitIndex);

        Game::showFramerate = options.showFramerate;

        #ifdef PLATFORM_GROUP_COMPUTER
        Game::fullscreen = options.fullscreen;
        #endif

        Camera::defaultZoomIndex = options.defaultCameraZoomIndex;
    }

    std::string serializeOptions(OptionsModel& options)
    {
        XMLPrinter printer;
        printer.OpenElement(ROOT_OPTIONS);
        printer.PushAttribute(ROOT_OPTIONS_ATTR_SCHEMA_VERSION, CURRENT_SCHEMA_VERSION);
        {
            printer.OpenElement(OPTION_SFX_VOLUME_PERCENT);
            printer.PushText(MathUtils::getPercentFromUnitInterval(options.sfxVolumeUnitInterval));
            printer.CloseElement();
        }
        {
            printer.OpenElement(OPTION_MUSIC_VOLUME_PERCENT);
            printer.PushText(MathUtils::getPercentFromUnitInterval(options.musicVolumeUnitInterval));
            printer.CloseElement();
        }
        #ifdef SUPPORTS_AIM_ASSIST
        {
            printer.OpenElement(OPTION_AIM_ASSIST);
            printer.PushText(options.aimAssist);
            printer.CloseElement();
        }
        #endif
        #ifdef SUPPORTS_CONTROLLER_RUMBLE
        {
            printer.OpenElement(OPTION_CONTROLLER_RUMBLE);
            printer.PushText(options.controllerRumble);
            printer.CloseElement();
        }
        #endif
        {
            printer.OpenElement(OPTION_FRAMERATE_LIMIT_INDEX);
            printer.PushText(options.framerateLimitIndex);
            printer.CloseElement();
        }
        {
            printer.OpenElement(OPTION_SHOW_FRAMERATE);
            printer.PushText(options.showFramerate);
            printer.CloseElement();
        }
        #ifdef PLATFORM_GROUP_COMPUTER
        {
            printer.OpenElement(OPTION_FULLSCREEN);
            printer.PushText(options.fullscreen);
            printer.CloseElement();
        }
        #endif
        {
            printer.OpenElement(OPTION_DEFAULT_CAMERA_ZOOM_INDEX);
            printer.PushText(options.defaultCameraZoomIndex);
            printer.CloseElement();
        }
        printer.CloseElement();
        const char* printerText = printer.CStr();
        return std::string(printerText);
    }

    OptionsModel deserializeOptions(std::string& serializedOptions)
    {
        OptionsModel options;

        XMLDocument doc;
        doc.Parse(serializedOptions.c_str());
        if (doc.Error())
        {
            return options;
        }

        XMLElement* root = doc.RootElement();

        int sfxVolumePercent = root->FirstChildElement(OPTION_SFX_VOLUME_PERCENT)
            ->IntText(MathUtils::getPercentFromUnitInterval(DefaultOptions::SFX_VOLUME));
        options.sfxVolumeUnitInterval = sfxVolumePercent / 100.0f;

        int musicVolumePercent = root->FirstChildElement(OPTION_MUSIC_VOLUME_PERCENT)
            ->IntText(MathUtils::getPercentFromUnitInterval(DefaultOptions::MUSIC_VOLUME));
        options.musicVolumeUnitInterval = musicVolumePercent / 100.0f;

        #ifdef SUPPORTS_AIM_ASSIST
        options.aimAssist = root->FirstChildElement(OPTION_AIM_ASSIST)
            ->BoolText(DefaultOptions::AIM_ASSIST);
        #endif

        #ifdef SUPPORTS_CONTROLLER_RUMBLE
        options.controllerRumble = root->FirstChildElement(OPTION_CONTROLLER_RUMBLE)
            ->BoolText(DefaultOptions::CONTROLLER_RUMBLE);
        #endif

        options.framerateLimitIndex = root->FirstChildElement(OPTION_FRAMERATE_LIMIT_INDEX)
            ->IntText(DefaultOptions::FRAMERATE_LIMIT_INDEX);

        options.showFramerate = root->FirstChildElement(OPTION_SHOW_FRAMERATE)
            ->BoolText(DefaultOptions::SHOW_FRAMERATE);

        #ifdef PLATFORM_GROUP_COMPUTER
        options.fullscreen = root->FirstChildElement(OPTION_FULLSCREEN)
            ->BoolText(DefaultOptions::FULLSCREEN);
        #endif

        options.defaultCameraZoomIndex = root->FirstChildElement(OPTION_DEFAULT_CAMERA_ZOOM_INDEX)
            ->IntText(DefaultOptions::DEFAULT_CAMERA_ZOOM_INDEX);

        return options;
    }
}
