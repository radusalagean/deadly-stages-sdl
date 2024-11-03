#include "../../Game.hpp"
#include "../Common/SaveDataHelper.hpp"

std::string resDirPathPrefix = "";

int main(int argc, char* argv[])
{
    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}