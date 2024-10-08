#ifndef __SRC_SCREEN_GAME_HUD_HPP__
#define __SRC_SCREEN_GAME_HUD_HPP__

#include "../../Game.hpp"

class HudHealthBar;

class Hud
{
public:
    Hud(int& score, int& wave, std::function<int()> getEnemiesLeft, const int& maxHealth, int& currentHealth);
    ~Hud();

    void load();
    void layoutPass();
    void update();
    void render();

private:
    int& score;
    int lastProcessedScore = -1;
    int& wave;
    int lastProcessedWave = -1;
    std::function<int()> getEnemiesLeft;
    int lastProcessedEnemiesLeft = -1;

    const std::string scoreLabel = "Score: ";
    const std::string waveLabel = "Wave: ";
    const std::string leftLabel = "Left: ";

    std::vector<Drawable*> drawables{};
    TextDrawable* scoreTextDrawable = nullptr;
    TextDrawable* waveTextDrawable = nullptr;
    TextDrawable* enemiesLeftTextDrawable = nullptr;
    HudHealthBar* healthBar = nullptr;

    std::string buildScoreText();
    std::string buildWaveText();
    std::string buildEnemiesLeftText();
};

#endif // __SRC_SCREEN_GAME_HUD_HPP__
