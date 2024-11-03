#include "XMLSerializer.hpp"

#include <tinyxml2.h>
#include "../Game.hpp"
#include "../Level/Camera.hpp"

using namespace tinyxml2;

namespace XMLSerializer // TODO Rename to OptionsAdapter
{
    std::string serializeOptions()
    {
        XMLPrinter printer;
        printer.OpenElement("options");
        printer.PushAttribute("schema_version", 1);
        {
            printer.OpenElement("sfx_volume_percent");
            float sfxVolumeUnitInterval = Game::audioManager.getSoundVolumeUnitInterval();
            int sfxVolumePercent = getPercentFromUnitInterval(sfxVolumeUnitInterval);
            printer.PushText(sfxVolumePercent);
            printer.CloseElement();
        }
        {
            printer.OpenElement("music_volume_percent");
            float musicVolumeUnitInterval = Game::audioManager.getMusicVolumeUnitInterval();
            int musicVolumePercent = getPercentFromUnitInterval(musicVolumeUnitInterval);
            printer.PushText(musicVolumePercent);
            printer.CloseElement();
        }
        #ifdef SUPPORTS_AIM_ASSIST
        {
            printer.OpenElement("aim_assist");
            printer.PushText(Game::control.aimAssist);
            printer.CloseElement();
        }
        #endif
        #ifdef SUPPORTS_CONTROLLER_RUMBLE
        {
            printer.OpenElement("controller_rumble");
            printer.PushText(Game::control.controllerRumble);
            printer.CloseElement();
        }
        #endif
        {
            printer.OpenElement("framerate_limit_index");
            printer.PushText(Game::getFramerateLimitIndex());
            printer.CloseElement();
        }
        {
            printer.OpenElement("show_framerate");
            printer.PushText(Game::showFramerate);
            printer.CloseElement();
        }
        #ifdef PLATFORM_GROUP_COMPUTER
        {
            printer.OpenElement("fullscreen");
            printer.PushText(Game::fullscreen);
            printer.CloseElement();
        }
        #endif
        {
            printer.OpenElement("default_camera_zoom_index");
            printer.PushText(Camera::defaultZoomIndex);
            printer.CloseElement();
        }
        printer.CloseElement();
        const char* printerText = printer.CStr();
        logd(printerText);
        return std::string(printerText);
    }

    int getPercentFromUnitInterval(float unitInterval)
    {
        int percent = round(unitInterval * 100);
        percent = std::max(0, std::min(percent, 100));
        return percent;
    }
};