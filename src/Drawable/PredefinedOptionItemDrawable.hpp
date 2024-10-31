#ifndef __SRC_DRAWABLE_PREDEFINEDOPTIONITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_PREDEFINEDOPTIONITEMDRAWABLE_HPP__

#include "OptionItemDrawable.hpp"

#include <string>
#include <functional>
#include <SDL.h>
#include <vector>
#include <utility>
#include "../Core/Debouncer.hpp"
#include "TextDrawable.hpp"

class PredefinedOptionItemDrawable : public OptionItemDrawable
{
private:
    enum NavigationAction
    {
        NAVIGATION_ACTION_LEFT = 0,
        NAVIGATION_ACTION_RIGHT
    };

    std::function<void(int)> callback;
    std::vector<std::string> options;
    int selectedOptionIndex;
    Debouncer navigationDebouncer;
    TextDrawable valueTextDrawable = TextDrawable("");
    SDL_Rect layoutRect;
    void refreshValueTextLayout();
    void onNavigationAction(NavigationAction action);

public:
    PredefinedOptionItemDrawable(const std::string& text, const std::vector<std::string>& options, 
        int selectedOptionIndex, std::function<void(int)> callback);
    void layout(int x, int y, int w, int h);
    void load();
    void handleEvents();
    void update();
    void draw();
};

#endif // __SRC_DRAWABLE_PREDEFINEDOPTIONITEMDRAWABLE_HPP__
