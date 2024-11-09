#include "../../Game.hpp"

#include "../../Platform/Common/SaveDataHelper.hpp"

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