#include "Hud.hpp"

#include "../../Core/Constants.hpp"
#include "../../Core/Macros.hpp"
#include "HudHealthBar.hpp"

Hud::Hud(int& score, int& wave, std::function<int()> getEnemiesLeft, const int& maxHealth, int& currentHealth) : 
    score(score), wave(wave), getEnemiesLeft(getEnemiesLeft), maxHealth(maxHealth), currentHealth(currentHealth)
{
    scoreTextDrawable = new TextDrawable(buildScoreText());
    drawables.push_back(scoreTextDrawable);
    waveTextDrawable = new TextDrawable(buildWaveText());
    drawables.push_back(waveTextDrawable);
    enemiesLeftTextDrawable = new TextDrawable(buildEnemiesLeftText());
    drawables.push_back(enemiesLeftTextDrawable);
    healthBar = new HudHealthBar(maxHealth, currentHealth);
    drawables.push_back(healthBar);
}

Hud::~Hud()
{
    dispose();
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
    return scoreLabel + std::to_string(score);
}

std::string Hud::buildWaveText()
{
    return waveLabel + std::to_string(wave);
}

std::string Hud::buildEnemiesLeftText()
{
    return leftLabel + std::to_string(getEnemiesLeft());
}

void Hud::layoutPass()
{
    const int midSeparation = USCALE(Game::width * 0.05);
    const float textHeight = USCALE(Game::height * 0.04);
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
    { // Health Bar
        Vector2D size = healthBar->computeSize();
        int width = size.getX();
        int height = size.getY();
        int x = Game::width / 2 - width / 2;
        int y = Game::height - height - Constants::WINDOW_PADDING_PX;
        healthBar->layout(x, y, width, height);
    }
}

void Hud::update()
{
    if (score != lastProcessedScore)
    {
        scoreTextDrawable->setText(buildScoreText());
        lastProcessedScore = score;
    }
    if (wave != lastProcessedWave)
    {
        waveTextDrawable->setText(buildWaveText());
        lastProcessedWave = wave;
    }
    if (getEnemiesLeft() != lastProcessedEnemiesLeft)
    {
        enemiesLeftTextDrawable->setText(buildEnemiesLeftText());
        lastProcessedEnemiesLeft = getEnemiesLeft();
    }
    for (auto drawable : drawables)
    {
        drawable->update();
    }
}

void Hud::render()
{
    for (auto drawable : drawables)
    {
        drawable->draw();
    }
}

void Hud::dispose()
{
    for (auto drawable : drawables)
    {
        drawable->dispose();
        delete drawable;
    }
    drawables.clear();
}