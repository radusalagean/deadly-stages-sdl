#include "PrimitiveShapeHelper.hpp"

#include <SDL.h>

void PrimitiveShapeHelper::drawRect(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color)
{
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}