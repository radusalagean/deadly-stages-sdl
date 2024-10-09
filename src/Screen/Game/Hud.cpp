#include "Hud.hpp"

#include "../../Core/Constants.hpp"
#include "../../Core/Macros.hpp"
#include "HudHealthBar.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"

Hud::Hud(Level& level) : level(level)
{
    scoreTextDrawable = new TextDrawable(buildScoreText());
    drawables.push_back(scoreTextDrawable);
    waveTextDrawable = new TextDrawable(buildWaveText());
    drawables.push_back(waveTextDrawable);
    enemiesLeftTextDrawable = new TextDrawable(buildEnemiesLeftText());
    drawables.push_back(enemiesLeftTextDrawable);
    playerHudDrawable = new ImageDrawable("player_hud.png");
    playerHudDrawable->transparency = 200;
    drawables.push_back(playerHudDrawable);
    healthBar = new HudHealthBar(level.player->maxHealth, level.player->currentHealth);
    drawables.push_back(healthBar);
}

Hud::~Hud()
{
    for (auto drawable : drawables)
    {
        delete drawable;
    }
    drawables.clear();
}

void Hud::load()
{
    for (auto drawable : drawables)
    {
        drawable->load();
    }
}

std::string Hud::buildScoreText()
{
    return scoreLabel + std::to_string(level.score);
}

std::string Hud::buildWaveText()
{
    return waveLabel + std::to_string(level.wave);
}

std::string Hud::buildEnemiesLeftText()
{
    return leftLabel + std::to_string(level.getEnemiesLeft());
}

void Hud::layoutPass()
{
    const int midSeparation = USCALE(Game::width * 0.05);
    const float textHeight = USCALE(Game::height * 0.04);
    playerHudPadding = USCALE(Game::height * 0.025);
    const float playerHudBarSpacing = USCALE(Game::height * 0.01);
    { // Score
        int width = textHeight * scoreTextDrawable->getAspectRatio();
        int x = Constants::WINDOW_PADDING_PX;
        int y = Constants::WINDOW_PADDING_PX;
        scoreTextDrawable->layout(x, y, width, textHeight);
    }
    { // Wave
        int width = textHeight * waveTextDrawable->getAspectRatio();
        int x = Game::width / 2 - width - midSeparation;
        int y = Constants::WINDOW_PADDING_PX;
        waveTextDrawable->layout(x, y, width, textHeight);
    }
    { // Enemies Left
        int width = textHeight * enemiesLeftTextDrawable->getAspectRatio();
        int x = Game::width / 2 + midSeparation;
        int y = Constants::WINDOW_PADDING_PX;
        enemiesLeftTextDrawable->layout(x, y, width, textHeight);
    }
    { // Player HUD
        int height = USCALE(Game::height * 0.22);
        int width = height * playerHudDrawable->getAspectRatio();
        int x = Constants::WINDOW_PADDING_PX;
        int y = Game::height - height - Constants::WINDOW_PADDING_PX;
        playerHudDrawable->layout(x, y, width, height);
    }
    { // Health Bar
        int width = playerHudDrawable->dstRect.w - 2 * playerHudPadding;
        int height = playerHudDrawable->dstRect.h * 0.3;
        int x = Constants::WINDOW_PADDING_PX + playerHudPadding;
        int y = Game::height - Constants::WINDOW_PADDING_PX - playerHudPadding - height;
        healthBar->layout(x, y, width, height);
    }
    { // Stamina Bar
        refreshStaminaBarWidth();
        staminaBarRect.h = playerHudDrawable->dstRect.h * 0.05;
        staminaBarRect.x = Constants::WINDOW_PADDING_PX + playerHudPadding;
        staminaBarRect.y = healthBar->dstRect.y - staminaBarRect.h - playerHudBarSpacing;
    }
}

void Hud::update()
{
    if (level.score != lastProcessedScore)
    {
        scoreTextDrawable->setText(buildScoreText());
        lastProcessedScore = level.score;
    }
    if (level.wave != lastProcessedWave)
    {
        waveTextDrawable->setText(buildWaveText());
        lastProcessedWave = level.wave;
    }
    if (level.getEnemiesLeft() != lastProcessedEnemiesLeft)
    {
        enemiesLeftTextDrawable->setText(buildEnemiesLeftText());
        lastProcessedEnemiesLeft = level.getEnemiesLeft();
    }
    for (auto drawable : drawables)
    {
        drawable->update();
    }
    refreshStaminaBarWidth();
}

void Hud::render()
{
    for (auto drawable : drawables)
    {
        drawable->draw();
    }
    Game::primitiveShapeHelper.drawRect(staminaBarRect, {240, 208, 26, 255});
}

void Hud::refreshStaminaBarWidth()
{
    staminaBarRect.w = healthBar->getActualWidth() * level.player->stamina;
}