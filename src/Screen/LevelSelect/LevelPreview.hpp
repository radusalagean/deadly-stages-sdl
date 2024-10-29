#ifndef __SRC_SCREEN_LEVELSELECT_LEVELPREVIEW_HPP__
#define __SRC_SCREEN_LEVELSELECT_LEVELPREVIEW_HPP__

#include "../../Drawable/ImageDrawable.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include <functional>
#include <string>
#include <SDL.h>

class LevelPreview
{
public:
    std::string id;
    std::string name;
    ImageDrawable* imageDrawable;
    bool selected;
    std::function<void()> callback;
    SDL_Rect selectionRect;
    int selectionRectOutlineThickness;

    LevelPreview(const std::string& id, const std::string& name, std::function<void()> callback);
    ~LevelPreview();

    void load();
    void update();
    void draw();
    void pick();
};

#endif // __SRC_SCREEN_LEVELSELECT_LEVELPREVIEW_HPP__
