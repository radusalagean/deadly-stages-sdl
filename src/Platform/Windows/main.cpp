#include "../../Game.hpp"

std::string resDirPathPrefix = "";

int WinMain()
{
    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}