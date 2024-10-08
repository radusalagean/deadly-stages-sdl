#ifndef __SRC_SCREEN_CREDITS_CREDITSSCREEN_HPP__
#define __SRC_SCREEN_CREDITS_CREDITSSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Drawable/TextDrawable.hpp"

class CreditsScreen : public Screen
{
private:
    struct Credit
    {
        std::string name;
        std::string nickname;
        std::string role;
    };

    struct CreditDrawable
    {
        TextDrawable* textDrawable;
        float heightUnitInterval;
        float paddingTopUnitInterval;
        float paddingBottomUnitInterval;
        int absoluteY = 0;
    };

    std::vector<CreditDrawable> drawables{};
    const std::vector<Credit> credits{
        {"Madalin-Iulian Popa", "XtremePrime", "2015 SFML Version Programmer"},
        {"Andrei-Sabin Popescu", "Popica23", "Graphics Artwork"},
        {"Radu Salagean", "", "2024 SDL Version Programmer"}
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
