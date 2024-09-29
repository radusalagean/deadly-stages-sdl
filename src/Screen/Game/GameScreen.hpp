#ifndef __SRC_SCREEN_GAME_GAMESCREEN_HPP__
#define __SRC_SCREEN_GAME_GAMESCREEN_HPP__

#include "../Screen.hpp"
#include "../../Level/LevelParser.hpp"

class Level;

class GameScreen : public Screen
{
private:
    std::string levelId;
    Level* level = nullptr;
    LevelParser levelParser;
    SDL_Texture* cursorTexture = nullptr;
    int cursorWidth = 0;
    int cursorHeight = 0;
    SDL_Rect cursorRect;

public:
    GameScreen(std::string levelId);
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
    void dispose();
};

#endif // __SRC_SCREEN_GAME_GAMESCREEN_HPP__
