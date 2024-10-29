#include "LevelSelectScreen.hpp"

#include "../../Debug/Logger.hpp"
#include "LevelPreview.hpp"
#include "../../Game.hpp"
#include "../../Control/Control.hpp"
#include "../../Core/Constants.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../Game/GameScreen.hpp"
#include "../../Core/Macros.hpp"
#include <algorithm>

LevelSelectScreen::LevelSelectScreen() : Screen()
{
    {
        std::string id = "openfield";
        std::string name = "Open Field";
        levelPreviews.push_back(new LevelPreview(id, name, std::bind(&LevelSelectScreen::selectLevel, this, id)));
    }
    {
        std::string id = "hell";
        std::string name = "Hell";
        levelPreviews.push_back(new LevelPreview(id, name, std::bind(&LevelSelectScreen::selectLevel, this, id)));
    }
    {
        std::string id = "future";
        std::string name = "Future";
        levelPreviews.push_back(new LevelPreview(id, name, std::bind(&LevelSelectScreen::selectLevel, this, id)));
    }
}

LevelSelectScreen::~LevelSelectScreen()
{
    for (auto levelPreview : levelPreviews)
    {
        delete levelPreview;
    }
    levelPreviews.clear();
}

void LevelSelectScreen::init()
{
    loadLevelPreviews();
    levelNameTextDrawable.load();
    loadingTextDrawable.load();
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
    if (!pendingLevelLoadId.empty()) 
        return;
    
    if (Game::control.isActionDown(CA_MENUS_BACK))
    {
        Game::screenManager.popScreen();
    }

    if (Game::control.isActionDown(CA_MENUS_LEFT) && navigationDebouncer.canPerformAction(NAVIGATION_ACTION_PREVIOUS))
    {
        previousLevel();
    }
    else if (Game::control.isActionUp(CA_MENUS_LEFT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_PREVIOUS);
    }

    if (Game::control.isActionDown(CA_MENUS_RIGHT) && navigationDebouncer.canPerformAction(NAVIGATION_ACTION_NEXT))
    {
        nextLevel();
    }
    else if (Game::control.isActionUp(CA_MENUS_RIGHT))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_NEXT);
    }
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_MENUS_SELECT, &pressedActionData))
    {
        levelPreviews[selectedIndex]->pick();
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void LevelSelectScreen::selectLevel(const std::string& id)
{
    pendingLevelLoadId = id;
}

void LevelSelectScreen::previousLevel()
{
    if (levelPreviews.empty()) return;
    selectedIndex = (selectedIndex - 1 + levelPreviews.size()) % levelPreviews.size();
}

void LevelSelectScreen::nextLevel()
{
    if (levelPreviews.empty()) return;
    selectedIndex = (selectedIndex + 1) % levelPreviews.size();
}

void LevelSelectScreen::layoutPass()
{
    int availableSpacePerLevelPreview = Game::width / levelPreviews.size();
    int imageWidth = availableSpacePerLevelPreview * 0.75;
    int imageHeight = imageWidth; // All preview images should be square
    int selectionRectOutlineThickness = (availableSpacePerLevelPreview - imageWidth) * 0.15;
    int selectionRectSpacing = selectionRectOutlineThickness;
    int remainingFreeSpacePerLevelPreview = availableSpacePerLevelPreview - (2 * selectionRectOutlineThickness + 2 * selectionRectSpacing) - imageWidth;
    int freeSpacePerSide = remainingFreeSpacePerLevelPreview / 2;
    int currentX = freeSpacePerSide;
    for (unsigned i = 0; i < levelPreviews.size(); i++)
    {
        LevelPreview* levelPreview = levelPreviews[i];
        levelPreview->selectionRect.y = Game::height / 2 - imageHeight / 2 - selectionRectOutlineThickness - selectionRectSpacing;
        levelPreview->selectionRect.x = currentX;
        levelPreview->selectionRect.w = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageWidth;
        levelPreview->selectionRect.h = 2 * (selectionRectOutlineThickness + selectionRectSpacing) + imageHeight;
        levelPreview->selectionRectOutlineThickness = selectionRectOutlineThickness;
        int imageX = levelPreview->selectionRect.x + selectionRectOutlineThickness + selectionRectSpacing;
        int imageY = levelPreview->selectionRect.y + selectionRectOutlineThickness + selectionRectSpacing;
        levelPreview->imageDrawable->layout(imageX, imageY, imageWidth, imageHeight);
        currentX += levelPreview->selectionRect.w + freeSpacePerSide;
    }

    layoutText();

    { // Loading...
        int textHeight = USCALE(Game::height * 0.045);
        int textWidth = textHeight * loadingTextDrawable.getAspectRatio();
        loadingTextDrawable.layout(
            Game::width / 2 - textWidth / 2, 
            Game::height / 2 - textHeight / 2,
            textWidth, 
            textHeight
        );
    }
    layoutInvalidated = false;
}

void LevelSelectScreen::layoutText()
{
    int textHeight = USCALE(Game::height * 0.045);
    int textWidth = textHeight * levelNameTextDrawable.getAspectRatio();
    int textX = Game::width / 2 - textWidth / 2;
    int textY = Game::height - Constants::WINDOW_PADDING_PX - textHeight;
    levelNameTextDrawable.layout(textX, textY, textWidth, textHeight);
}

void LevelSelectScreen::update()
{
    if (!pendingLevelLoadId.empty() && loadingIndicatorRendered)
    {
        Game::screenManager.setScreen(new GameScreen(pendingLevelLoadId));
        return;
    }
    for (unsigned i = 0; i < levelPreviews.size(); i++)
    {
        levelPreviews[i]->selected = i == selectedIndex;
        levelPreviews[i]->update();
    }
    if (levelNameTextDrawable.getText() != levelPreviews[selectedIndex]->name)
    {
        levelNameTextDrawable.setText(levelPreviews[selectedIndex]->name);
        layoutText();
    }
    levelNameTextDrawable.update();
    loadingTextDrawable.update();
}

void LevelSelectScreen::render()
{
    if (!pendingLevelLoadId.empty())
    {
        loadingTextDrawable.draw();
        loadingIndicatorRendered = true;
        return;
    }
    for (auto levelPreview : levelPreviews)
    {
        levelPreview->draw();
    }
    levelNameTextDrawable.draw();
}