#include "GameScreen.hpp"

#include "../../Debug/Logger.hpp"
#include "../../Game.hpp"
#include <SDL.h>
#include "../../Level/Level.hpp"
#include "../../Screen/MainMenu/MainMenuScreen.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Core/SDLUtils.hpp"
#include "../../Core/Macros.hpp"

GameScreen::GameScreen(std::string levelId) : levelId(levelId)
{

}

GameScreen::~GameScreen()
{
    delete hud;
    delete level;

#ifdef SUPPORTS_MOUSE_POINTER
    SDL_DestroyTexture(cursorTexture);
#endif
}

void GameScreen::init()
{
    level = levelParser.parseLevel(levelId);
    level->load();
    hud = new Hud(*level);
    hud->load();
    pauseOverlay.load();

#ifdef SUPPORTS_MOUSE_POINTER
    cursorTexture = SDLUtils::loadTexture("res/image/crosshair.png");
    SDL_QueryTexture(cursorTexture, nullptr, nullptr, &cursorWidth, &cursorHeight);
#endif
}

void GameScreen::handleEvents()
{
#ifdef SUPPORTS_MOUSE_POINTER
    SDL_GetMouseState(&cursorRect.x, &cursorRect.y);
#endif

    if (paused)
    {
        pauseOverlay.handleEvents();
        return;
    }
    level->handleEvents();
    const PressedActionData* pressedActionData = nullptr;
    if (Game::control.isActionDown(CA_GAME_PAUSE, &pressedActionData))
    {
        paused = true;
        Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(*pressedActionData);
        Game::audioManager.pauseMusic();
    }
}

void GameScreen::layoutPass()
{
    hud->layoutPass();
    pauseOverlay.layoutPass();
    layoutInvalidated = false;
}

void GameScreen::update()
{
    if (paused)
    {
        pauseOverlay.update();
        return;
    }
    level->update();
    hud->update();
}

void GameScreen::render()
{
    // Level    
    level->render();

    // HUD
    hud->render();

    // Pause overlay
    if (paused)
    {
        pauseOverlay.render();
    }

#ifdef SUPPORTS_MOUSE_POINTER
    if (Game::control.getCurrentControlSource() == CS_KEYBOARD_AND_MOUSE)
    {
        // Custom cursor
        cursorRect.w = USCALE(cursorWidth);
        cursorRect.h = USCALE(cursorHeight);
        // Center the crosshair on the cursor
        cursorRect.x = cursorRect.x - cursorRect.w / 2;
        cursorRect.y = cursorRect.y - cursorRect.h / 2;
        SDL_RenderCopy(Game::renderer, cursorTexture, nullptr, &cursorRect);
    }
#endif
}
