#include "LevelPreview.hpp"

#include "../../Game.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"

LevelPreview::LevelPreview(const std::string& id, const std::string& name, std::function<void()> callback)
    : id(id), name(name), selected(false), callback(callback)
{
    this->imageDrawable = new ImageDrawable("preview.png", "res/level/" + id + "/");
    this->nameTextDrawable = new TextDrawable(name);
}

void LevelPreview::load()
{
    imageDrawable->load();
    nameTextDrawable->load();
}

void LevelPreview::update()
{
    imageDrawable->update();
    nameTextDrawable->update();
}

void LevelPreview::draw()
{
    if (selected)
    {
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, selectionRect, {255, 0, 0, 255}, 
            selectionRectOutlineThickness);
    }
    imageDrawable->draw();
    nameTextDrawable->draw();
}

void LevelPreview::dispose()
{
    imageDrawable->dispose();
    delete imageDrawable;
    nameTextDrawable->dispose();
    delete nameTextDrawable;
}

void LevelPreview::pick()
{
    callback();
}