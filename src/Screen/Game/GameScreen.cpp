#include "GameScreen.hpp"

#include "../../Debug/Logger.hpp"
#include "../../Game.hpp"
#include <SDL.h>
#include "../../Level/Level.hpp"
#include "../../Screen/MainMenu/MainMenuScreen.hpp"
#include "../../ScreenManager/ScreenManager.hpp"

GameScreen::GameScreen(std::string levelId) : levelId(levelId)
{

}

void GameScreen::init()
{
    level = levelParser.parseLevel(levelId);
    level->load();
}

void GameScreen::handleEvents()
{
    level->handleEvents();
    if (Game::controls.isActionDown(CA_ESCAPE))
    {
        Game::screenManager.setScreen(new MainMenuScreen());
    }
}

void GameScreen::layoutPass()
{

}

void GameScreen::update()
{
    level->update();
}

void GameScreen::render()
{
    SDL_RenderClear(Game::renderer);
    level->render();
    SDL_RenderPresent(Game::renderer);
}

void GameScreen::dispose()
{
    delete level;
}