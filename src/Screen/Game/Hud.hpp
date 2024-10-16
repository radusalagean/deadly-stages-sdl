#ifndef __SRC_SCREEN_GAME_HUD_HPP__
#define __SRC_SCREEN_GAME_HUD_HPP__

#include "../../Game.hpp"
#include "../../Level/Level.hpp"
#include "../../GameEntity/Weapon.hpp"

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

    bool playerHudLaidOut = false;

    int lastProcessedScore = -1;
    int lastProcessedWave = -1;
    int lastProcessedEnemiesLeft = -1;

    const std::string scoreLabel = "Score: ";
    const std::string waveLabel = "Wave: ";
    const std::string leftLabel = "Enemies: ";

    float playerHudPadding = 0.0f;
    int playerHudVisibleTopLeftXPadded = 0;

    std::vector<Drawable*> drawables{};
    TextDrawable* scoreTextDrawable = nullptr;
    TextDrawable* waveTextDrawable = nullptr;
    TextDrawable* enemiesLeftTextDrawable = nullptr;

    ImageDrawable* playerHudDrawable = nullptr;
    
    HudHealthBar* healthBar = nullptr;

    int lastProcessedWeaponId = WeaponId::WEAPON_UNDEFINED;
    ImageDrawable* weaponIconDrawable = nullptr;
    TextDrawable* ammoInCurrentMagTextDrawable = nullptr;
    int lastProcessedAmmoInCurrentMag = -1;
    TextDrawable* availableMagsTextDrawable = nullptr;
    int lastProcessedAvailableMags = -1;
    void layoutWeaponStats();
    void layoutAvailableMagsText();

    SDL_Rect staminaBarRect;
    SDL_Rect infiniteMagsRect;
    int infiniteMagsWidth = 0;

    std::string buildScoreText();
    std::string buildWaveText();
    std::string buildEnemiesLeftText();

    void refreshStaminaBarWidth();
    void refreshWeaponStats();
};

#endif // __SRC_SCREEN_GAME_HUD_HPP__
