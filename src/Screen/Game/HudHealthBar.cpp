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
    setDstRect(x, y, w, h);
}

void HudHealthBar::load()
{

}

void HudHealthBar::update()
{
    cellWidth = dstRect.w / numCells;
}

void HudHealthBar::draw()
{
    int currentX = dstRect.x;
    int cellHeight = dstRect.h;
    for (int i = 0; i < numCells; i++)
    {
        drawCell(currentX, dstRect.y, cellWidth, cellHeight, i < filledCells);
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
    Game::primitiveShapeHelper.drawRect(outerRect, backgroundColor);
    int borderWidth = ceil(borderWidthUnitInterval * h);
    Game::primitiveShapeHelper.drawRectOutline(outerRect, borderColor, borderWidth);
    SDL_Rect innerRect;
    int innerRectHeight = ceil(h * innerCellHeightUnitInterval);
    if (innerRectHeight % 2 == 1)
        innerRectHeight--;
    innerRect.x = x;
    innerRect.y = y + (h - innerRectHeight) / 2;
    innerRect.w = w;
    innerRect.h = innerRectHeight;
    SDL_Color innerColor = isFull ? healthColor : backgroundColor;
    Game::primitiveShapeHelper.drawRect(innerRect, innerColor);
    Game::primitiveShapeHelper.drawRectOutline(innerRect, borderColor, borderWidth);
}

int HudHealthBar::getActualWidth()
{
    return cellWidth * numCells;
}