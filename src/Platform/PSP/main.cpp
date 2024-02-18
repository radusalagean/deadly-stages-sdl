#include "../../Game.hpp"

int main(int argc, char *argv[])
{
    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}