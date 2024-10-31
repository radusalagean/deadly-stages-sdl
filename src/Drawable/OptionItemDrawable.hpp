#ifndef __SRC_DRAWABLE_OPTIONITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_OPTIONITEMDRAWABLE_HPP__

#include "Drawable.hpp"

#include <string>

class TextDrawable;

class OptionItemDrawable : public Drawable
{
private:
    SDL_Rect selectionRect;
    TextDrawable* textDrawable = nullptr;

protected:
    int contentPadding = 0;

public:
    OptionItemDrawable(const std::string& text);
    virtual ~OptionItemDrawable();
    bool selected = false;
    virtual void layout(int x, int y, int w, int h);
    virtual void load();
    virtual void handleEvents();
    virtual void update();
    virtual void draw();
};

#endif // __SRC_DRAWABLE_OPTIONITEMDRAWABLE_HPP__
