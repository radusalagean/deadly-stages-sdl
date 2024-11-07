#ifndef __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
#define __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__

#include <SDL.h>

struct Line;

class PrimitiveShapeHelper
{
public:
    void drawRect(const SDL_Rect& rect, const SDL_Color& color);
    void drawRectOutline(const SDL_Rect& rect, const SDL_Color& color, int borderWidth);
    SDL_Rect wrapRects(const SDL_Rect& rect1, const SDL_Rect& rect2);
    void drawLine(const Line& line, const SDL_Color& color);
    void drawArrowTriangle(SDL_Rect& dstRect, int direction, const SDL_Color& color, int thickness);
};

#endif // __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
