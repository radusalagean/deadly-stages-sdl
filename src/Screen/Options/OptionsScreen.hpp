#ifndef __SRC_SCREEN_OPTIONS_OPTIONSSCREEN_HPP__
#define __SRC_SCREEN_OPTIONS_OPTIONSSCREEN_HPP__

#include "../Screen.hpp"
#include "../../Drawable/OptionItemDrawable.hpp"
#include "../../Drawable/SliderOptionItemDrawable.hpp"
#include "../../Core/Debouncer.hpp"
#include <vector>

class OptionsScreen : public Screen
{
private:
    std::vector<OptionItemDrawable*> optionsItemDrawables;
    int selectedOptionItemIndex = 0;
    Debouncer navigationDebouncer;

    enum NavigationAction
    {
        NAVIGATION_ACTION_PREVIOUS = 0,
        NAVIGATION_ACTION_NEXT
    };

public:
    OptionsScreen();
    ~OptionsScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_OPTIONS_OPTIONSSCREEN_HPP__
