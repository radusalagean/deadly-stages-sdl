#include "OptionItemDrawable.hpp"

#include "TextDrawable.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

OptionItemDrawable::OptionItemDrawable(const std::string& text)
{
    textDrawable = new TextDrawable(text);
}

OptionItemDrawable::~OptionItemDrawable()
{
    delete textDrawable;
}

void OptionItemDrawable::layout(int x, int y, int w, int h)
{
    int selctionRectSize = h;
    selectionRect.x = x;
    selectionRect.y = y;
    selectionRect.w = selctionRectSize;
    selectionRect.h = selctionRectSize;
    int textX = x + selctionRectSize + selctionRectSize * 0.5f;
    int textHeight = h;
    int textWidth = h * textDrawable->getAspectRatio();
    textDrawable->layout(textX, y, textWidth, textHeight);
}

void OptionItemDrawable::load()
{
    textDrawable->load();
}

void OptionItemDrawable::handleEvents()
{
}

void OptionItemDrawable::update()
{
    if (selected)
    {
        textDrawable->setColor(Constants::COLOR_YELLOW_ACCENT);
    }
    else
    {
        textDrawable->setColor(Constants::COLOR_WHITE);
    }
    textDrawable->update();
}

void OptionItemDrawable::draw()
{
    if (selected)
    {
        Game::primitiveShapeHelper.drawRect(selectionRect, Constants::COLOR_YELLOW_ACCENT);
    }
    textDrawable->draw();
}