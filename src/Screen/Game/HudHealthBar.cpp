#include "HudHealthBar.hpp"

#include "../../Core/PrimitiveShapeHelper.hpp"
#include "../../Game.hpp"
#include "../../Core/Macros.hpp"
#include <cmath>

HudHealthBar::HudHealthBar(const int& numCells, int& filledCells) : Drawable(), numCells(numCells), filledCells(filledCells)
{
    
}

HudHealthBar::~HudHealthBar()
{

}

void HudHealthBar::layout(int x, int y, int w, int h)
{
    allocatedRect.x = x;
    allocatedRect.y = y;
    allocatedRect.w = w;
    allocatedRect.h = h;
}

void HudHealthBar::load()
{

}

void HudHealthBar::update()
{

}

void HudHealthBar::draw()
{
    int currentX = allocatedRect.x;
    int cellWidth = allocatedRect.w / numCells;
    for (int i = 0; i < numCells; i++)
    {
        drawCell(currentX, allocatedRect.y, cellWidth, allocatedRect.h, i < filledCells);
        currentX += cellWidth;
    }
}

void HudHealthBar::drawCell(int x, int y, int w, int h, bool isFull)
{
    SDL_Rect outerRect;
    if (h % 2 == 1)
        h--;
    outerRect.x = x;
    outerRect.y = y;
    outerRect.w = w;
    outerRect.h = h;
    Game::primitiveShapeHelper.drawRect(Game::renderer, outerRect, backgroundColor);
    int borderWidth = ceil(borderWidthUnitInterval * h);
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, outerRect, borderColor, borderWidth);
    SDL_Rect innerRect;
    int innerRectHeight = ceil(h * innerCellHeightUnitInterval);
    if (innerRectHeight % 2 == 1)
        innerRectHeight--;
    innerRect.x = x;
    innerRect.y = y + (h - innerRectHeight) / 2;
    innerRect.w = w;
    innerRect.h = innerRectHeight;
    SDL_Color innerColor = isFull ? healthColor : backgroundColor;
    Game::primitiveShapeHelper.drawRect(Game::renderer, innerRect, innerColor);
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, innerRect, borderColor, borderWidth);
}

Vector2D HudHealthBar::computeSize()
{
    return Vector2D(numCells * USCALE(cellWidth), USCALE(cellHeight)) * (Game::height / 512.0f);
}
