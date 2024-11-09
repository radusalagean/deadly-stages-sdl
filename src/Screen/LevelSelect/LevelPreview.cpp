#include "LevelPreview.hpp"

#include "../../Game.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"
#include "../../Core/Constants.hpp"

LevelPreview::LevelPreview(const std::string& id, const std::string& name, std::function<void()> callback)
    : id(id), name(name), selected(false), callback(callback)
{
    int highScore = Game::highScores.getScore(id);
    this->highScoreDrawable = new TextDrawable(highScore > 0 ? "High Score: " + std::to_string(highScore) : "", Constants::COLOR_YELLOW_ACCENT);
    this->imageDrawable = new ImageDrawable("preview.png", "res/level/" + id + "/");
    this->nameDrawable = new TextDrawable(name);
}

LevelPreview::~LevelPreview()
{
    delete highScoreDrawable;
    delete imageDrawable;
    delete nameDrawable;
}

void LevelPreview::load()
{
    highScoreDrawable->load();
    imageDrawable->load();
    nameDrawable->load();
}

void LevelPreview::update()
{
    highScoreDrawable->update();
    imageDrawable->update();
    nameDrawable->update();
}

void LevelPreview::draw()
{
    if (selected)
    {
        highScoreDrawable->draw();
        Game::primitiveShapeHelper.drawRectOutline(selectionRect, Constants::COLOR_YELLOW_ACCENT, selectionRectOutlineThickness);
        nameDrawable->draw();
    }
    imageDrawable->draw();
}

void LevelPreview::pick()
{
    callback();
}