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

    refreshWeaponStats();
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
        playerHudLaidOut = true;
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
    layoutWeaponStats();
}

void Hud::layoutWeaponStats()
{
    if (!playerHudLaidOut)
        return;
    float horizontalSpacing = USCALE(Game::height * 0.02);
    { // Weapon icon
        int height = playerHudDrawable->dstRect.h * 0.35;
        int width = height * weaponIconDrawable->getAspectRatio();
        int x = Constants::WINDOW_PADDING_PX + playerHudPadding;
        int y = playerHudDrawable->dstRect.y + playerHudPadding;
        weaponIconDrawable->layout(x, y, width, height);
    }
    
    { // Ammo in current mag
        float textHeight = weaponIconDrawable->dstRect.h * 0.5;
        int height = textHeight;
        int width = height * ammoInCurrentMagTextDrawable->getAspectRatio();
        int x = weaponIconDrawable->dstRect.x + weaponIconDrawable->dstRect.w + horizontalSpacing;
        int y = weaponIconDrawable->dstRect.y;
        ammoInCurrentMagTextDrawable->layout(x, y, width, height);
    }
    // Available mags
    layoutAvailableMagsText();
    { // Infinite mags
        infiniteMagsRect.h = availableMagsTextDrawable->dstRect.h;
        infiniteMagsRect.w = 2.5 * infiniteMagsRect.h;
        infiniteMagsRect.x = playerHudVisibleTopLeftXPadded - infiniteMagsRect.w;
        infiniteMagsRect.y = availableMagsTextDrawable->dstRect.y;
        infiniteMagsWidth = std::ceil(infiniteMagsRect.h * 0.1);
    }
}

void Hud::layoutAvailableMagsText()
{
    float textHeight = weaponIconDrawable->dstRect.h * 0.5;
    int height = textHeight;
    int width = height * availableMagsTextDrawable->getAspectRatio();
    playerHudVisibleTopLeftXPadded = playerHudDrawable->dstRect.x + playerHudDrawable->dstRect.w * 0.3648 - playerHudPadding;
    int x = playerHudVisibleTopLeftXPadded - width;
    int y = weaponIconDrawable->dstRect.y + weaponIconDrawable->dstRect.h - height;
    availableMagsTextDrawable->layout(x, y, width, height);
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
    refreshWeaponStats();
}

void Hud::render()
{
    for (auto drawable : drawables)
    {
        drawable->draw();
    }
    Game::primitiveShapeHelper.drawRect(staminaBarRect, Constants::COLOR_YELLOW_ACCENT);
    if (level.currentPlayerWeapon->hasInfiniteMags)
    {
        Game::primitiveShapeHelper.drawInfinitySymbol(infiniteMagsRect, Constants::COLOR_YELLOW_ACCENT, infiniteMagsWidth);
    }
}

void Hud::refreshStaminaBarWidth()
{
    staminaBarRect.w = healthBar->getActualWidth() * level.player->stamina;
}

void Hud::refreshWeaponStats()
{
    bool weaponChanged = lastProcessedWeaponId != level.currentPlayerWeapon->id;
    if (weaponChanged)
    {
        // Weapon icon
        logd("Refreshing weapon icon");
        SDL_Texture* currentWeaponTexture = level.currentPlayerWeapon->texture;
        if (weaponIconDrawable != nullptr)
        {
            drawables.erase(std::remove(drawables.begin(), drawables.end(), weaponIconDrawable), drawables.end());
            delete weaponIconDrawable;
            weaponIconDrawable = nullptr;
        }
        weaponIconDrawable = new ImageDrawable(currentWeaponTexture);
        weaponIconDrawable->load();
        drawables.push_back(weaponIconDrawable);

        // Delete info about ammo in current mag
        if (ammoInCurrentMagTextDrawable != nullptr)
        {
            drawables.erase(std::remove(drawables.begin(), drawables.end(), ammoInCurrentMagTextDrawable), drawables.end());
            delete ammoInCurrentMagTextDrawable;
            ammoInCurrentMagTextDrawable = nullptr;
            lastProcessedAmmoInCurrentMag = -1;
        }

        // Delete info about available mags
        if (availableMagsTextDrawable != nullptr)
        {
            drawables.erase(std::remove(drawables.begin(), drawables.end(), availableMagsTextDrawable), drawables.end());
            delete availableMagsTextDrawable;
            availableMagsTextDrawable = nullptr;
            lastProcessedAvailableMags = -1;
        }
    }
    
    // Ammo in current mag
    std::string ammoInCurrentMagString = std::to_string(level.currentPlayerWeapon->ammoInCurrentMag);
    if (ammoInCurrentMagTextDrawable == nullptr)
    {
        logd("Creating ammo in current mag text drawable");
        ammoInCurrentMagTextDrawable = new TextDrawable(ammoInCurrentMagString);
        ammoInCurrentMagTextDrawable->load();
        drawables.push_back(ammoInCurrentMagTextDrawable);
    }
    else if (lastProcessedAmmoInCurrentMag != level.currentPlayerWeapon->ammoInCurrentMag)
    {
        ammoInCurrentMagTextDrawable->setText(ammoInCurrentMagString);
    }
    lastProcessedAmmoInCurrentMag = level.currentPlayerWeapon->ammoInCurrentMag;

    // Available mags
    std::string availableMagsString = level.currentPlayerWeapon->hasInfiniteMags ? "" : std::to_string(level.currentPlayerWeapon->availableMags);
    if (availableMagsTextDrawable == nullptr)
    {
        logd("Creating available mags text drawable");
        availableMagsTextDrawable = new TextDrawable(availableMagsString, Constants::COLOR_YELLOW_ACCENT, "PressStart2P.ttf");
        availableMagsTextDrawable->load();
        drawables.push_back(availableMagsTextDrawable);
    }
    else if (lastProcessedAvailableMags != level.currentPlayerWeapon->availableMags)
    {
        availableMagsTextDrawable->setText(availableMagsString);
        layoutAvailableMagsText();
    }
    lastProcessedAvailableMags = level.currentPlayerWeapon->availableMags;

    // Update last processed weapon id
    lastProcessedWeaponId = level.currentPlayerWeapon->id;

    if (weaponChanged)
    {
        layoutWeaponStats();
    }
}