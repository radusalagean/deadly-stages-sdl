#ifndef __SRC_LAYOUT_LAYOUT_HPP__
#define __SRC_LAYOUT_LAYOUT_HPP__

#include <unordered_map>
#include <string>
#include <SDL.h>

enum LayoutType
{

};

class Layout
{
public:
    Layout();
    ~Layout();

    void layout();

private:
    bool invalidated = true;

    std::unordered_map<std::string, SDL_Rect*> rects;
};

#endif // __SRC_LAYOUT_LAYOUT_HPP__
