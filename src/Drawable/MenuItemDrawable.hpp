#ifndef __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__
#define __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <functional>

#include "Drawable.hpp"

class TextDrawable;

class MenuItemDrawable : public Drawable
{
private:
    TextDrawable* textDrawable = nullptr;
    SDL_Rect selectionRect;
    std::function<void()> callback;

public:
    ~MenuItemDrawable();

    bool selected = false;
    MenuItemDrawable(const std::string& text, std::function<void()> callback);
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
    void pick();
};
    

#endif // __SRC_DRAWABLE_MENUITEMDRAWABLE_HPP__
