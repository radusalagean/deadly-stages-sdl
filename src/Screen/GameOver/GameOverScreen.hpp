#ifndef __SRC_SCREEN_GAMEOVER_GAMEOVERSCREEN_HPP__
#define __SRC_SCREEN_GAMEOVER_GAMEOVERSCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "../../Drawable/ImageDrawable.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Drawable/MenuDrawable.hpp"
#include "../Screen.hpp"

class GameOverScreen : public Screen
{
private:
    const std::string levelId;
    
    std::vector<Drawable*> drawables{};
    #pragma region Drawables
    ImageDrawable* gameOverImageDrawable = nullptr;
    TextDrawable* statsTextDrawable = nullptr;
    MenuDrawable* menuDrawable = nullptr;
    #pragma endregion

    void loadAssets();
    void loadMenuItems();

    void menuContinue();
    void menuEnd();

public:
    GameOverScreen(int score, int wave, const std::string& levelId);
    ~GameOverScreen();
    void init();
    void handleEvents();
    void layoutPass();
    void update();
    void render();
};

#endif // __SRC_SCREEN_GAMEOVER_GAMEOVERSCREEN_HPP__
