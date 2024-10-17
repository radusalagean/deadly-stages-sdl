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
    SDL_ShowCursor(SDL_ENABLE);
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
    SDL_ShowCursor(SDL_DISABLE);
    cursorTexture = SDLUtils::loadTexture("res/image/cursor.png");
    SDL_QueryTexture(cursorTexture, NULL, NULL, &cursorWidth, &cursorHeight);
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
    if (Game::control.isActionDown(CA_MENUS_BACK))
    {
        paused = true;
        Game::control.releaseAndBlockAction(CA_MENUS_BACK);
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
    // Custom cursor
    cursorRect.w = USCALE(cursorWidth);
    cursorRect.h = USCALE(cursorHeight);
    // Center the crosshair on the cursor
    cursorRect.x = cursorRect.x - cursorRect.w / 2;
    cursorRect.y = cursorRect.y - cursorRect.h / 2;
    SDL_RenderCopy(Game::renderer, cursorTexture, NULL, &cursorRect);
#endif
}
