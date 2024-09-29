#include "LevelSelectScreen.hpp"

#include "../../Debug/Logger.hpp"
#include "LevelPreview.hpp"
#include "../../Game.hpp"
#include "../../Controls/Controls.hpp"
#include "../../Core/Constants.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../Game/GameScreen.hpp"
#include "../../Core/Macros.hpp"
#include <algorithm>

LevelSelectScreen::LevelSelectScreen() : Screen()
{
   {
        std::string id = "devtest";
        std::string name = "DevTest Level";
        levelPreviews.push_back(new LevelPreview(id, name, std::bind(&LevelSelectScreen::selectLevel, this, id)));
    }
    {
        std::string id = "demons";
        std::string name = "Demons Level";
        levelPreviews.push_back(new LevelPreview(id, name, std::bind(&LevelSelectScreen::selectLevel, this, id)));
    }
}

void LevelSelectScreen::init()
{
    loadLevelPreviews();
}

void LevelSelectScreen::loadLevelPreviews()
{
    for (auto levelPreview : levelPreviews)
    {
        levelPreview->load();
    }
}

void LevelSelectScreen::handleEvents()
{
    if (Game::controls.isActionDown(CA_ESCAPE))
    {
        Game::screenManager.popScreen();
    }

    if (Game::controls.isActionDown(CA_LEFT) && navigationDebouncer.canPerformAction(NAVIGATION_ACTION_PREVIOUS))
    {
        previousLevel();
    }
    else if (Game::controls.isActionUp(CA_LEFT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_PREVIOUS);
    }

    if (Game::controls.isActionDown(CA_RIGHT) && navigationDebouncer.canPerformAction(NAVIGATION_ACTION_NEXT))
    {
        nextLevel();
    }
    else if (Game::controls.isActionUp(CA_RIGHT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_NEXT);
    }
    if (Game::controls.isActionDown(CA_SELECT))
    {
        levelPreviews[selectedIndex]->pick();
    }
}

void LevelSelectScreen::selectLevel(const std::string& id)
{
    logd("Selected level: %s", id.c_str());
    Game::screenManager.setScreen(new GameScreen(id));
}

void LevelSelectScreen::previousLevel()
{
    if (levelPreviews.empty()) return;
    logd("Previous level");
    selectedIndex = (selectedIndex - 1 + levelPreviews.size()) % levelPreviews.size();
}

void LevelSelectScreen::nextLevel()
{
    if (levelPreviews.empty()) return;
    logd("Next level");
    selectedIndex = (selectedIndex + 1) % levelPreviews.size();
}

void LevelSelectScreen::layoutPass()
{
    int textHeight = USCALE(Game::height * 0.045);
    int imageHeight = USCALE(Game::height * 0.4);
    int selectionRectOutlineThickness = USCALE(Game::height * 0.02);
    int selectionRectSpacing = USCALE(Game::height * 0.02);
    int textSpacing = USCALE(Game::height * 0.025);
    { // DevTest Level
        LevelPreview* levelPreview = levelPreviews[0];
        int imageWidth = imageHeight * levelPreview->imageDrawable->getAspectRatio();
        levelPreview->selectionRect.x = Constants::WINDOW_PADDING_PX;
        levelPreview->selectionRect.y = Constants::WINDOW_PADDING_PX;
        levelPreview->selectionRect.w = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageWidth;
        levelPreview->selectionRect.h = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageHeight;
        levelPreview->selectionRectOutlineThickness = selectionRectOutlineThickness;
        int imageX = levelPreview->selectionRect.x + selectionRectOutlineThickness + selectionRectSpacing;
        int imageY = levelPreview->selectionRect.y + selectionRectOutlineThickness + selectionRectSpacing;
        levelPreview->imageDrawable->layout(imageX, imageY, imageWidth, imageHeight);
        int textWidth = textHeight * levelPreview->nameTextDrawable->getAspectRatio();
        int textX = std::max(Constants::WINDOW_PADDING_PX, std::min(Game::width - Constants::WINDOW_PADDING_PX - textWidth, imageX + (imageWidth - textWidth) / 2));
        int textY = levelPreview->selectionRect.y + levelPreview->selectionRect.h + textSpacing;
        levelPreview->nameTextDrawable->layout(textX, textY, textWidth, textHeight);
    }
    { // Demons Level
        LevelPreview* levelPreview = levelPreviews[1];
        int imageWidth = imageHeight * levelPreview->imageDrawable->getAspectRatio();
        levelPreview->selectionRect.w = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageWidth;
        levelPreview->selectionRect.h = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageHeight;
        levelPreview->selectionRect.x = Game::width - Constants::WINDOW_PADDING_PX - levelPreview->selectionRect.w;
        levelPreview->selectionRect.y = Game::height - Constants::WINDOW_PADDING_PX - levelPreview->selectionRect.h;
        levelPreview->selectionRectOutlineThickness = selectionRectOutlineThickness;
        int imageY = Game::height - Constants::WINDOW_PADDING_PX - selectionRectOutlineThickness - selectionRectSpacing - imageHeight;
        int imageX = Game::width - Constants::WINDOW_PADDING_PX - selectionRectOutlineThickness - selectionRectSpacing - imageWidth;
        levelPreview->imageDrawable->layout(imageX, imageY, imageWidth, imageHeight);
        int textWidth = textHeight * levelPreview->nameTextDrawable->getAspectRatio();
        int textX = std::max(Constants::WINDOW_PADDING_PX, std::min(Game::width - Constants::WINDOW_PADDING_PX - textWidth, imageX + (imageWidth - textWidth) / 2));
        int textY = levelPreview->selectionRect.y - textHeight - textSpacing;
        levelPreview->nameTextDrawable->layout(textX, textY, textWidth, textHeight);
    }
}

void LevelSelectScreen::update()
{
    for (unsigned i = 0; i < levelPreviews.size(); i++)
    {
        levelPreviews[i]->selected = i == selectedIndex;
        levelPreviews[i]->update();
    }
}

void LevelSelectScreen::render()
{
    SDL_RenderClear(Game::renderer);
    for (auto levelPreview : levelPreviews)
    {
        levelPreview->draw();
    }
    SDL_RenderPresent(Game::renderer);
}

void LevelSelectScreen::dispose()
{
    for (auto levelPreview : levelPreviews)
    {
        levelPreview->dispose();
        delete levelPreview;
    }
    levelPreviews.clear();
}