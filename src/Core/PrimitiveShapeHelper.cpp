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

void PrimitiveShapeHelper::drawRectOutline(SDL_Renderer* renderer, const SDL_Rect& rect, const SDL_Color& color, int borderWidth)
{
    SDL_Color oldColor;
    SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    
    // Draw top border
    SDL_Rect topBorder = {rect.x, rect.y, rect.w, borderWidth};
    SDL_RenderFillRect(renderer, &topBorder);
    
    // Draw bottom border
    SDL_Rect bottomBorder = {rect.x, rect.y + rect.h - borderWidth, rect.w, borderWidth};
    SDL_RenderFillRect(renderer, &bottomBorder);
    
    // Draw left border
    SDL_Rect leftBorder = {rect.x, rect.y + borderWidth, borderWidth, rect.h - 2 * borderWidth};
    SDL_RenderFillRect(renderer, &leftBorder);
    
    // Draw right border
    SDL_Rect rightBorder = {rect.x + rect.w - borderWidth, rect.y + borderWidth, borderWidth, rect.h - 2 * borderWidth};
    SDL_RenderFillRect(renderer, &rightBorder);
    
    SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}
