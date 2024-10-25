#include "Hud.hpp"

#include "../../Core/Constants.hpp"
#include "../../Core/Macros.hpp"
#include "HudHealthBar.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"
#include "../../GameEntity/Player.hpp"

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
        int width = height * ammoInWeaponTextDrawable->getAspectRatio();
        int x = weaponIconDrawable->dstRect.x + weaponIconDrawable->dstRect.w + horizontalSpacing;
        int y = weaponIconDrawable->dstRect.y;
        ammoInWeaponTextDrawable->layout(x, y, width, height);
    }
    // Available ammo
    layoutAvailableAmmoText();
    { // Infinite ammo
        infiniteAmmoRect.h = availableAmmoTextDrawable->dstRect.h;
        infiniteAmmoRect.w = 2.5 * infiniteAmmoRect.h;
        infiniteAmmoRect.x = playerHudVisibleTopLeftXPadded - infiniteAmmoRect.w;
        infiniteAmmoRect.y = availableAmmoTextDrawable->dstRect.y;
        infiniteAmmoWidth = std::ceil(infiniteAmmoRect.h * 0.1);
    }
}

void Hud::layoutAvailableAmmoText()
{
    float textHeight = weaponIconDrawable->dstRect.h * 0.5;
    int height = textHeight;
    int width = height * availableAmmoTextDrawable->getAspectRatio();
    playerHudVisibleTopLeftXPadded = playerHudDrawable->dstRect.x + playerHudDrawable->dstRect.w * 0.3648 - playerHudPadding;
    int x = playerHudVisibleTopLeftXPadded - width;
    int y = weaponIconDrawable->dstRect.y + weaponIconDrawable->dstRect.h - height;
    availableAmmoTextDrawable->layout(x, y, width, height);
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
    if (level.currentPlayerWeapon->hasInfiniteAmmo)
    {
        Game::primitiveShapeHelper.drawInfinitySymbol(infiniteAmmoRect, Constants::COLOR_YELLOW_ACCENT, infiniteAmmoWidth);
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

        // Delete info about ammo in weapon
        if (ammoInWeaponTextDrawable != nullptr)
        {
            drawables.erase(std::remove(drawables.begin(), drawables.end(), ammoInWeaponTextDrawable), drawables.end());
            delete ammoInWeaponTextDrawable;
            ammoInWeaponTextDrawable = nullptr;
            lastProcessedAmmoInWeapon = -1;
        }

        // Delete info about available ammo
        if (availableAmmoTextDrawable != nullptr)
        {
            drawables.erase(std::remove(drawables.begin(), drawables.end(), availableAmmoTextDrawable), drawables.end());
            delete availableAmmoTextDrawable;
            availableAmmoTextDrawable = nullptr;
            lastProcessedAvailablAmmo = -1;
        }
    }
    
    // Ammo in weapon
    std::string ammoInWeaponString = std::to_string(level.currentPlayerWeapon->ammoInWeapon);
    if (ammoInWeaponTextDrawable == nullptr)
    {
        ammoInWeaponTextDrawable = new TextDrawable(ammoInWeaponString);
        ammoInWeaponTextDrawable->load();
        drawables.push_back(ammoInWeaponTextDrawable);
    }
    else if (lastProcessedAmmoInWeapon != level.currentPlayerWeapon->ammoInWeapon)
    {
        ammoInWeaponTextDrawable->setText(ammoInWeaponString);
    }
    lastProcessedAmmoInWeapon = level.currentPlayerWeapon->ammoInWeapon;

    // Ammo in inventory
    std::string ammoInInventoryString = level.currentPlayerWeapon->hasInfiniteAmmo ? "" : std::to_string(level.currentPlayerWeapon->ammoInInventory);
    if (availableAmmoTextDrawable == nullptr)
    {
        availableAmmoTextDrawable = new TextDrawable(ammoInInventoryString, Constants::COLOR_YELLOW_ACCENT, "PressStart2P.ttf");
        availableAmmoTextDrawable->load();
        drawables.push_back(availableAmmoTextDrawable);
    }
    else if (lastProcessedAvailablAmmo != level.currentPlayerWeapon->ammoInInventory)
    {
        availableAmmoTextDrawable->setText(ammoInInventoryString);
        layoutAvailableAmmoText();
    }
    lastProcessedAvailablAmmo = level.currentPlayerWeapon->ammoInInventory;

    // Update last processed weapon id
    lastProcessedWeaponId = level.currentPlayerWeapon->id;

    if (weaponChanged)
    {
        layoutWeaponStats();
    }
}