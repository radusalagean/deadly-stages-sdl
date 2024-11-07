#include "PrimitiveShapeHelper.hpp"

#include <SDL.h>
#include <algorithm>
#include "../Game.hpp"
#include "Line.hpp"
#include "SDLUtils.hpp"

void PrimitiveShapeHelper::drawRect(const SDL_Rect& rect, const SDL_Color& color)
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
    
    SDLUtils::popTempRendererDrawColor();
}

void PrimitiveShapeHelper::drawRectOutline(const SDL_Rect& rect, const SDL_Color& color, int borderWidth)
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    
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
    
    SDLUtils::popTempRendererDrawColor();
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

void PrimitiveShapeHelper::drawLine(const Line& line, const SDL_Color& color)
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);

    SDLUtils::popTempRendererDrawColor();
}

void PrimitiveShapeHelper::drawArrowTriangle(SDL_Rect& dstRect, int direction, const SDL_Color& color, int thickness)
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int arrowSize = dstRect.h / 2;
    int arrowHeadSize = arrowSize / 2;
    int arrowBaseSize = arrowSize - arrowHeadSize;

    int centerX = dstRect.x + dstRect.w / 2;
    int centerY = dstRect.y + dstRect.h / 2;

    // Draw multiple offset lines to create thickness
    for (int i = -thickness / 2; i <= thickness / 2; i++) 
    {
        if (direction == 0) // Up
        {
            SDL_RenderDrawLine(renderer, centerX + i, centerY + arrowSize, centerX + i, centerY);
            SDL_RenderDrawLine(renderer, centerX - arrowHeadSize + i, centerY + arrowBaseSize, centerX + i, centerY + arrowSize);
            SDL_RenderDrawLine(renderer, centerX + arrowHeadSize + i, centerY + arrowBaseSize, centerX + i, centerY + arrowSize);
        }
        else if (direction == 1) // Right
        {
            SDL_RenderDrawLine(renderer, centerX, centerY + i, centerX + arrowSize, centerY + i);
            SDL_RenderDrawLine(renderer, centerX + arrowBaseSize, centerY - arrowHeadSize + i, centerX + arrowSize, centerY + i);
            SDL_RenderDrawLine(renderer, centerX + arrowBaseSize, centerY + arrowHeadSize + i, centerX + arrowSize, centerY + i);
        }
        else if (direction == 2) // Down
        {
            SDL_RenderDrawLine(renderer, centerX + i, centerY, centerX + i, centerY + arrowSize);
            SDL_RenderDrawLine(renderer, centerX - arrowHeadSize + i, centerY + arrowBaseSize, centerX + i, centerY);
            SDL_RenderDrawLine(renderer, centerX + arrowHeadSize + i, centerY + arrowBaseSize, centerX + i, centerY);
        }
        else if (direction == 3) // Left
        {
            SDL_RenderDrawLine(renderer, centerX, centerY + i, centerX - arrowSize, centerY + i);
            SDL_RenderDrawLine(renderer, centerX - arrowBaseSize, centerY - arrowHeadSize + i, centerX - arrowSize, centerY + i);
            SDL_RenderDrawLine(renderer, centerX - arrowBaseSize, centerY + arrowHeadSize + i, centerX - arrowSize, centerY + i);
        }
    }

    SDLUtils::popTempRendererDrawColor();
}