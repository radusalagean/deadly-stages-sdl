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
    SDL_ShowCursor(SDL_ENABLE);
}

void GameScreen::init()
{
    level = levelParser.parseLevel(levelId);
    level->load();
    hud = new Hud(level->score, level->wave, std::bind(&Level::getEnemiesLeft, level), level->player->maxHealth, level->player->currentHealth);
    hud->load();
    // Custom cursor
    SDL_ShowCursor(SDL_DISABLE);
    cursorTexture = SDLUtils::loadTexture("res/image/cursor.png");
    SDL_QueryTexture(cursorTexture, NULL, NULL, &cursorWidth, &cursorHeight);
}

void GameScreen::handleEvents()
{
    level->handleEvents();
    if (Game::control.isActionDown(CA_ESCAPE))
    {
        Game::screenManager.setScreen(new MainMenuScreen());
    }

    // Custom cursor
    SDL_GetMouseState(&cursorRect.x, &cursorRect.y);
}

void GameScreen::layoutPass()
{
    hud->layoutPass();
    layoutInvalidated = false;
}

void GameScreen::update()
{
    level->update();
    hud->update();
}

void GameScreen::render()
{
    SDL_RenderClear(Game::renderer);

    // Level    
    level->render();

    // Custom cursor
    cursorRect.w = USCALE(cursorWidth);
    cursorRect.h = USCALE(cursorHeight);
    // Center the crosshair on the cursor
    cursorRect.x = cursorRect.x - cursorRect.w / 2;
    cursorRect.y = cursorRect.y - cursorRect.h / 2;
    SDL_RenderCopy(Game::renderer, cursorTexture, NULL, &cursorRect);

    // HUD
    hud->render();

    SDL_RenderPresent(Game::renderer);
}
