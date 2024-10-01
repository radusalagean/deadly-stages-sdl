#include "PrimitiveShapeHelper.hpp"

#include <SDL.h>
#include <algorithm>

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

SDL_Rect PrimitiveShapeHelper::wrapRects(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    // Find the top-left corner of the new rect
    int x_min = std::min(rect1.x, rect2.x);
    int y_min = std::min(rect1.y, rect2.y);

    // Find the bottom-right corner of the new rect
    int x_max = std::max(rect1.x + rect1.w, rect2.x + rect2.w);
    int y_max = std::max(rect1.y + rect1.h, rect2.y + rect2.h);

    SDL_Rect wrapped;
    wrapped.x = x_min;
    wrapped.y = y_min;
    wrapped.w = x_max - x_min;
    wrapped.h = y_max - y_min;

    return wrapped;
}
