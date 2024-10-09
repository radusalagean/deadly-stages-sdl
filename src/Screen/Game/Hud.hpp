#ifndef __SRC_SCREEN_GAME_HUD_HPP__
#define __SRC_SCREEN_GAME_HUD_HPP__

#include "../../Game.hpp"
#include "../../Level/Level.hpp"

class HudHealthBar;

class Hud
{
public:
    Hud(Level& level);
    ~Hud();

    void load();
    void layoutPass();
    void update();
    void render();

private:
    Level& level;

    int lastProcessedScore = -1;
    int lastProcessedWave = -1;
    int lastProcessedEnemiesLeft = -1;

    const std::string scoreLabel = "Score: ";
    const std::string waveLabel = "Wave: ";
    const std::string leftLabel = "Enemies: ";

    float playerHudPadding = 0.0f;

    std::vector<Drawable*> drawables{};
    TextDrawable* scoreTextDrawable = nullptr;
    TextDrawable* waveTextDrawable = nullptr;
    TextDrawable* enemiesLeftTextDrawable = nullptr;
    ImageDrawable* playerHudDrawable = nullptr;
    HudHealthBar* healthBar = nullptr;
    SDL_Rect staminaBarRect;

    std::string buildScoreText();
    std::string buildWaveText();
    std::string buildEnemiesLeftText();

    void refreshStaminaBarWidth();
};

#endif // __SRC_SCREEN_GAME_HUD_HPP__
