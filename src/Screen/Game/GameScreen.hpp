#ifndef __SRC_SCREEN_GAME_GAMESCREEN_HPP__
#define __SRC_SCREEN_GAME_GAMESCREEN_HPP__

#include "../Screen.hpp"
#include "../../Level/LevelParser.hpp"
#include "Hud.hpp"
#include "PauseOverlay.hpp"
#include "../../Core/Config.hpp"

class Level;

class GameScreen : public Screen
{
private:
    bool paused = false;
    std::string levelId;
    Level* level = nullptr;
    Hud* hud = nullptr;
    LevelParser levelParser;

#ifdef SUPPORTS_MOUSE_POINTER
    SDL_Texture* cursorTexture = nullptr;
    int cursorWidth = 0;
    int cursorHeight = 0;
    SDL_Rect cursorRect;
#endif

    PauseOverlay pauseOverlay = PauseOverlay(paused);

public:
    GameScreen(std::string levelId);
    ~GameScreen();
    
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_GAME_GAMESCREEN_HPP__
