#ifndef __SRC_SCREEN_CREDITS_CREDITSSCREEN_HPP__
#define __SRC_SCREEN_CREDITS_CREDITSSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include <cfloat>

class CreditsScreen : public Screen
{
private:
    struct Credit
    {
        std::string creditOwner;
        std::string nickname = "";
        std::string role;
        std::string link = "";
        std::string license = "";
    };

    struct CreditDrawable
    {
        TextDrawable* textDrawable;
        float heightUnitInterval;
        float paddingTopUnitInterval;
        float paddingBottomUnitInterval;
        int absoluteY = 0;
    };

    std::vector<std::vector<CreditDrawable>> drawables{};
    const std::vector<Credit> credits{
        {"Madalin-Iulian Popa", "XtremePrime", "2015 Original Version Programmer"},
        {"Andrei-Sabin Popescu", "Popica23", "Graphics Artwork"},
        {"Radu Salagean", "", "2024 Remix Programmer & SFX"},
        {"Stefania Pruteanu", "09RZW", "DVD & Case design"},
        {"Michel Baradari", "", "SFX", "apollo-music.de", "Licensed under CC BY 3.0"},
        {"Guns by Gary", "", "SFX", "fossilrecords.net", "Licensed under CC BY-SA 3.0"},
        {"Tim Mortimer", "", "SFX", "archive.org/details/TimMortimer", "Licensed under CC BY 3.0"},
        {"pixbay.com", "", "SFX", "", "Licensed under Pixbay Content License"},
        {"One Man Symphony", "", "Music", "onemansymphony.bandcamp.com", "Reflective District - Licensed under CC BY 4.0"}
    };
    const std::string footerText = "Thanks for playing!";

    float scrollOffsetPx = FLT_MAX;
    int contentHeightPx = 0;
    const float scrollSpeedUnitIntervalPerSecond = 0.07f;
    int lastKnownWindowHeightPx = 0;

    void loadAssets();

public:
    CreditsScreen();
    ~CreditsScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_CREDITS_CREDITSSCREEN_HPP__
