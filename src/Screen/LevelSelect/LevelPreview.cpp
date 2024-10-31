#include "LevelPreview.hpp"

#include "../../Game.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"
#include "../../Core/Constants.hpp"

LevelPreview::LevelPreview(const std::string& id, const std::string& name, std::function<void()> callback)
    : id(id), name(name), selected(false), callback(callback)
{
    this->imageDrawable = new ImageDrawable("preview.png", "res/level/" + id + "/");
}

LevelPreview::~LevelPreview()
{
    delete imageDrawable;
}

void LevelPreview::load()
{
    imageDrawable->load();
}

void LevelPreview::update()
{
    imageDrawable->update();
}

void LevelPreview::draw()
{
    if (selected)
    {
        Game::primitiveShapeHelper.drawRectOutline(selectionRect, Constants::COLOR_YELLOW_ACCENT, selectionRectOutlineThickness);
    }
    imageDrawable->draw();
}

void LevelPreview::pick()
{
    callback();
}