#ifndef __SRC_DRAWABLE_BOOLEANOPTIONITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_BOOLEANOPTIONITEMDRAWABLE_HPP__

#include "OptionItemDrawable.hpp"

#include <string>
#include <functional>
#include <SDL.h>

class BooleanOptionItemDrawable : public OptionItemDrawable
{
private:
    std::function<void(bool)> callback;
    SDL_Rect checkboxRect;
    int checkboxOutlineThickness;
    bool currentValue;
    void toggle();

public:
    BooleanOptionItemDrawable(const std::string& text, bool currentValue, std::function<void(bool)> callback);
    void layout(int x, int y, int w, int h);
    void load();
    void handleEvents();
    void update();
    void draw();
};

#endif // __SRC_DRAWABLE_BOOLEANOPTIONITEMDRAWABLE_HPP__
