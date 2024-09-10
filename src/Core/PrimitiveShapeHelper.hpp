#ifndef __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
#define __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__

#include <SDL.h>

class PrimitiveShapeHelper
{
public:
    void drawRect(SDL_Renderer* renderer, const SDL_Rect& rect, 
        const SDL_Color& color);
};

#endif // __SRC_CORE_PRIMITIVESHAPEHELPER_HPP__
