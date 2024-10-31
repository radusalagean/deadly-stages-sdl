#ifndef __SRC_DRAWABLE_CLICKABLEOPTIONITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_CLICKABLEOPTIONITEMDRAWABLE_HPP__

#include "OptionItemDrawable.hpp"

#include <string>
#include <functional>
#include <SDL.h>

class ClickableOptionItemDrawable : public OptionItemDrawable
{
private:
    std::function<void()> callback;

public:
    ClickableOptionItemDrawable(const std::string& text, std::function<void()> callback);
    void layout(int x, int y, int w, int h);
    void load();
    void handleEvents();
    void update();
    void draw();
};

#endif // __SRC_DRAWABLE_CLICKABLEOPTIONITEMDRAWABLE_HPP__
