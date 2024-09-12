#ifndef __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
#define __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__

#include <SDL.h>

class PrimitiveShapeHelper
{
public:
    void drawRect(SDL_Renderer* renderer, const SDL_Rect& rect, 
        const SDL_Color& color);
    void drawRectOutline(SDL_Renderer* renderer, const SDL_Rect& rect, 
        const SDL_Color& color, int borderWidth);
};

#endif // __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
