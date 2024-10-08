#include "MenuItemDrawable.hpp"

#include "../Game.hpp"
#include "../Core/FontManager.hpp"
#include <cassert>
#include "../Core/PrimitiveShapeHelper.hpp"
#include <algorithm>

#define SELECTION_RECT_SPACING_FACTOR 0.5

MenuItemDrawable::~MenuItemDrawable()
{
    delete textDrawable;
}

MenuItemDrawable::MenuItemDrawable(const std::string& text, std::function<void()> callback) :
    callback(callback)
{
    textDrawable = new TextDrawable(text);
}

void MenuItemDrawable::load()
{
    textDrawable->load();
    sourceHeight = textDrawable->getSourceHeight();
    sourceWidth = sourceHeight + sourceHeight * SELECTION_RECT_SPACING_FACTOR 
        + textDrawable->getSourceWidth();
    aspectRatio = (float)sourceWidth / (float)sourceHeight;
}

void MenuItemDrawable::layout(int x, int y, int w, int h)
{
    int selctionRectSize = h;
    selectionRect.x = x;
    selectionRect.y = y;
    selectionRect.w = selctionRectSize;
    selectionRect.h = selctionRectSize;
    int textX = x + selctionRectSize + float(selctionRectSize * 0.5);
    int textWidth = h * textDrawable->getAspectRatio();
    textDrawable->layout(textX, y, textWidth, h);
}

void MenuItemDrawable::update()
{
}

void MenuItemDrawable::draw()
{
    if (selected)
    {
        Game::primitiveShapeHelper.drawRect(Game::renderer, selectionRect, {255, 255, 255, 255});
    }
    textDrawable->draw();
}

void MenuItemDrawable::pick()
{
    callback();
}