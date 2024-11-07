#include "ControlsScreen.hpp"

#include "../../Core/SDLUtils.hpp"
#include "../../Game.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Core/Macros.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"

ControlsScreen::ControlsScreen()
{
    #ifdef PLATFORM_GROUP_COMPUTER
    if (Game::control.getCurrentControlSource() == CS_KEYBOARD_AND_MOUSE)
    {
        currentControlPageIndex = 0;
    }
    else
    {
        currentControlPageIndex = 1;
    }
    #endif
}

ControlsScreen::~ControlsScreen()
{
    for (auto& controlPageDrawable : controlPageDrawables)
    {
        delete controlPageDrawable.titleTextDrawable;
        for (auto& controlEntryDrawable : controlPageDrawable.controlEntryDrawables)
        {
            delete controlEntryDrawable.actionTextDrawable;
            for (auto& iconDrawable : controlEntryDrawable.iconDrawables)
            {
                delete iconDrawable;
            }
        }
    }
    controlPageDrawables.clear();

    #ifdef PLATFORM_GROUP_COMPUTER
    if (keyboardTexture)
        SDL_DestroyTexture(keyboardTexture);
    if (mouseTexture)
        SDL_DestroyTexture(mouseTexture);
    if (xboxControllerTexture)
        SDL_DestroyTexture(xboxControllerTexture);
    #elif PLATFORM_PSP
    if (playstationControllerTexture)
        SDL_DestroyTexture(playstationControllerTexture);
    #endif
    if (sticksTexture)
        SDL_DestroyTexture(sticksTexture);
}

void ControlsScreen::init()
{
    #ifdef PLATFORM_GROUP_COMPUTER
    keyboardTexture = SDLUtils::loadTexture("res/image/control/kb_light_all.png");
    mouseTexture = SDLUtils::loadTexture("res/image/control/mouse.png");
    xboxControllerTexture = SDLUtils::loadTexture("res/image/control/controller_xbox.png");
    #elif PLATFORM_PSP
    playstationControllerTexture = SDLUtils::loadTexture("res/image/control/controller_ps.png");
    #endif
    sticksTexture = SDLUtils::loadTexture("res/image/control/sticks.png");

    for (auto& controlPage : controlPages)
    {
        TextDrawable* titleTextDrawable = new TextDrawable(controlPage.title, Constants::COLOR_YELLOW_ACCENT);
        std::vector<ControlEntryDrawable> controlEntryDrawables{};
        for (auto& controlEntry : controlPage.entries)
        {
            std::vector<ImageDrawable*> iconDrawables{};
            for (auto& spriteSheetItem : controlEntry.spriteSheetItems)
            {
                ImageDrawable* iconDrawable = new ImageDrawable(getTextureFromControlTextureId(spriteSheetItem.textureId));
                iconDrawable->setSrcRect(spriteSheetItem.columnIndex * GRID_CELL_SIZE_PX, 
                    spriteSheetItem.rowIndex * GRID_CELL_SIZE_PX, 
                    GRID_CELL_SIZE_PX, GRID_CELL_SIZE_PX);
                iconDrawables.push_back(iconDrawable);
            }
            TextDrawable* actionTextDrawable = new TextDrawable(controlEntry.label);
            controlEntryDrawables.push_back({iconDrawables, actionTextDrawable});
        }
        ControlPageDrawable controlPageDrawable;
        controlPageDrawable.titleTextDrawable = titleTextDrawable;
        controlPageDrawable.controlEntryDrawables = controlEntryDrawables;
        controlPageDrawables.push_back(controlPageDrawable);
    }

    for (auto& controlPageDrawable : controlPageDrawables)
    {
        controlPageDrawable.titleTextDrawable->load();
        for (auto& controlEntryDrawable : controlPageDrawable.controlEntryDrawables)
        {
            for (auto& iconDrawable : controlEntryDrawable.iconDrawables)
            {
                iconDrawable->load();
            }
            controlEntryDrawable.actionTextDrawable->load();
        }
    }
}

void ControlsScreen::handleEvents()
{
    if (Game::control.isActionDown(CA_MENUS_BACK))
    {
        Game::screenManager.popScreen();
    }
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_MENUS_LEFT, &pressedActionData))
    {
        cycleControlPage(-1);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
    else if (Game::control.isActionDown(CA_MENUS_RIGHT, &pressedActionData))
    {
        cycleControlPage(1);
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
    }
}

void ControlsScreen::layoutPass()
{
    arrowThickness = ceil(USCALE(Game::height * 0.005f));
    int titleHeight = USCALE(Game::height * 0.05f);
    int titleBottomSpacing = titleHeight * 0.5f;
    int entryRowHeight = USCALE(Game::height * 0.08f);
    int entryRowSpacing = entryRowHeight * 0.1f;
    int actionTextHeight = entryRowHeight * 0.5f;
    int iconSize = entryRowHeight;
    int textYOffset = (iconSize - actionTextHeight) / 2;
    for (auto& controlPageDrawable : controlPageDrawables)
    {
        int currentY = Constants::WINDOW_PADDING_PX;
        if (controlPageDrawable.titleTextDrawable->getText().length() > 0)
        {
            int titleWidth = titleHeight * controlPageDrawable.titleTextDrawable->getAspectRatio();
            int titleX = (Game::width - titleWidth) / 2;
            controlPageDrawable.titleTextDrawable->layout(titleX, currentY, titleWidth, titleHeight);
            int arrowSize = titleHeight;
            controlPageDrawable.leftArrowDstRect = {titleX - arrowSize, currentY, arrowSize, arrowSize};
            controlPageDrawable.rightArrowDstRect = {titleX + titleWidth, currentY, arrowSize, arrowSize};
            currentY += titleHeight + titleBottomSpacing;
        }
        

        for (auto& controlEntryDrawable : controlPageDrawable.controlEntryDrawables)
        {
            int actionTextWidth = controlEntryDrawable.actionTextDrawable->getAspectRatio() * actionTextHeight;
            int rowWidth = controlEntryDrawable.iconDrawables.size() * iconSize + actionTextWidth;
            int currentX = (Game::width - rowWidth) / 2;
            for (auto& iconDrawable : controlEntryDrawable.iconDrawables)
            {
                iconDrawable->layout(currentX, currentY, iconSize, iconSize);
                currentX += iconSize;
            }
            controlEntryDrawable.actionTextDrawable->layout(currentX, currentY + textYOffset, actionTextWidth, actionTextHeight);
            currentY += entryRowHeight + entryRowSpacing;
        }
    }
}

void ControlsScreen::update()
{
    for (auto& controlPageDrawable : controlPageDrawables)
    {
        controlPageDrawable.titleTextDrawable->update();
        for (auto& controlEntryDrawable : controlPageDrawable.controlEntryDrawables)
        {
            for (auto& iconDrawable : controlEntryDrawable.iconDrawables)
            {
                iconDrawable->update();
            }
            controlEntryDrawable.actionTextDrawable->update();
        }
    }
}

void ControlsScreen::render()
{
    ControlPageDrawable currentControlPageDrawable = controlPageDrawables[currentControlPageIndex];
    if (controlPageDrawables.size() > 1)
    {
        Game::primitiveShapeHelper.drawArrowTriangle(currentControlPageDrawable.leftArrowDstRect, 3, Constants::COLOR_YELLOW_ACCENT, arrowThickness);
        Game::primitiveShapeHelper.drawArrowTriangle(currentControlPageDrawable.rightArrowDstRect, 1, Constants::COLOR_YELLOW_ACCENT, arrowThickness);
    }
    currentControlPageDrawable.titleTextDrawable->draw();
    for (auto& controlEntryDrawable : currentControlPageDrawable.controlEntryDrawables)
    {
        for (auto& iconDrawable : controlEntryDrawable.iconDrawables)
        {
            iconDrawable->draw();
        }
        controlEntryDrawable.actionTextDrawable->draw();
    }
}

SDL_Texture* ControlsScreen::getTextureFromControlTextureId(ControlTextureId textureId)
{
    switch (textureId)
    {
        #ifdef PLATFORM_GROUP_COMPUTER
        case ControlTextureId::KEYBOARD:
            return keyboardTexture;
        case ControlTextureId::MOUSE:
            return mouseTexture;
        case ControlTextureId::XBOX_CONTROLLER:
            return xboxControllerTexture;
        #elif PLATFORM_PSP
        case ControlTextureId::PLAYSTATION_CONTROLLER:
            return playstationControllerTexture;
        #endif
        case ControlTextureId::STICKS:
            return sticksTexture;
        default:
            return nullptr;
    }
}

void ControlsScreen::cycleControlPage(int delta)
{
    if (controlPageDrawables.size() <= 1)
        return;
    currentControlPageIndex = (currentControlPageIndex + delta + controlPageDrawables.size()) % controlPageDrawables.size();
    if (currentControlPageIndex < 0)
        currentControlPageIndex += controlPageDrawables.size();
}
