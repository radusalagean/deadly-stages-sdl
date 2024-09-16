#include "GameScreen.hpp"

#include "../../Debug/Logger.hpp"
#include "../../Game.hpp"
#include <SDL.h>
#include "../../Level/Level.hpp"

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
    if (Game::controls.isActionDown(CA_UP))
    {
        velocity.setY(-1);
    }
    else if (Game::controls.isActionDown(CA_DOWN))
    {
        velocity.setY(1);
    }
    else if (Game::controls.isActionDown(CA_LEFT))
    {
        velocity.setX(-1);
    }
    else if (Game::controls.isActionDown(CA_RIGHT))
    {
        velocity.setX(1);
    }
    else
    {
        velocity.setX(0);
        velocity.setY(0);
    }
    camera.target += velocity;
}

void GameScreen::layoutPass()
{

}

void GameScreen::update()
{

}

void GameScreen::render()
{
    SDL_RenderClear(Game::renderer);
    level->render(camera);
    SDL_RenderPresent(Game::renderer);
}

void GameScreen::dispose()
{

}