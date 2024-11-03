#ifndef __SRC_SCREEN_PERMANENTOVERLAY_PERMANENTOVERLAYSCREEN_HPP__
#define __SRC_SCREEN_PERMANENTOVERLAY_PERMANENTOVERLAYSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Core/Constants.hpp"
#include "../../Core/FramerateIndicator.hpp"

class PermanentOverlayScreen : public Screen
{
private:
    TextDrawable loadingTextDrawable = TextDrawable("Loading...", Constants::COLOR_YELLOW_ACCENT);
    TextDrawable savingTextDrawable = TextDrawable("Saving...", Constants::COLOR_YELLOW_ACCENT);
    FramerateIndicator framerateIndicator;

public:
    PermanentOverlayScreen();
    ~PermanentOverlayScreen();
    void init();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_PERMANENTOVERLAY_PERMANENTOVERLAYSCREEN_HPP__
