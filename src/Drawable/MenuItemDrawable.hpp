#ifndef __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Drawable.hpp"

class TextDrawable;

class MenuItemDrawable : public Drawable
{
private:
    TextDrawable* textDrawable = nullptr;
    SDL_Rect selectionRect;

public:
    bool selected = false;
    MenuItemDrawable(const std::string& text);
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
    void dispose();
};
    

#endif // __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__
