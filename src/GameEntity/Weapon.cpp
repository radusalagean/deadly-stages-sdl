#include "Weapon.hpp"

#include <stdlib.h>
#include "../Core/FloatAnimator.hpp"
#include "../Control/Control.hpp"
#include "../Game.hpp"
#include "../Core/TexturePool.hpp"
#include "../Core/AudioManager.hpp"

WeaponConfig Weapon::createWeaponConfig(WeaponId id)
{
    WeaponConfig cfg;
    cfg.id = id;
    switch (id)
    {
        case WeaponId::WEAPON_PISTOL:
            cfg.ammoType = AmmoType::AMMO_TYPE_9MM_BULLET;
            cfg.projectileType = ProjectileType::PROJECTILE_TYPE_9MM_BULLET;
            cfg.damagePerProjectile = 1;
            cfg.maxAmmoCapacity = 8;
            cfg.hasInfiniteAmmo = true;
            cfg.fireRateDelayMillis = 50;
            cfg.reloadTimeMillis = 1739;
            cfg.automatic = false;
            cfg.projectilesPerShot = 1;
            cfg.spreadAngle = 0;
            cfg.ammoPerReloadCycle = cfg.maxAmmoCapacity; // A full mag is loaded per reload cycle
            cfg.textureFileName = "pickup_pistol.png";
            cfg.rumbleIntensity = 0.4f;
            cfg.rumbleDurationMs = 64;
            cfg.chamberNeedsManualFill = false;
            cfg.chamberManualFillPostFireDelayMillis = 0;
            break;
        case WeaponId::WEAPON_SHOTGUN:
            cfg.ammoType = AmmoType::AMMO_TYPE_00_BUCK_SHELL;
            cfg.projectileType = ProjectileType::PROJECTILE_TYPE_00_BUCK_SHELL_PELLET;
            cfg.damagePerProjectile = 1;
            cfg.maxAmmoCapacity = 5;
            cfg.hasInfiniteAmmo = false;
            cfg.fireRateDelayMillis = 859;
            cfg.reloadTimeMillis = 300;
            cfg.automatic = false;
            cfg.projectilesPerShot = 9; // A 00 Buck shotgun shell fires 9 pellets (I looked it up on YouTube)
            cfg.spreadAngle = 15;
            cfg.ammoPerReloadCycle = 1; // A single shell is loaded per reload cycle
            cfg.textureFileName = "pickup_shotgun.png";
            cfg.rumbleIntensity = 0.8f;
            cfg.rumbleDurationMs = 128;
            cfg.chamberNeedsManualFill = true;
            cfg.chamberManualFillPostFireDelayMillis = 150;
            break;
        case WeaponId::WEAPON_SMG:
            cfg.ammoType = AmmoType::AMMO_TYPE_9MM_BULLET;
            cfg.projectileType = ProjectileType::PROJECTILE_TYPE_9MM_BULLET;
            cfg.damagePerProjectile = 1;
            cfg.maxAmmoCapacity = 30;
            cfg.hasInfiniteAmmo = false;
            cfg.fireRateDelayMillis = 76;
            cfg.reloadTimeMillis = 1740;
            cfg.automatic = true;
            cfg.projectilesPerShot = 1;
            cfg.spreadAngle = 0;
            cfg.ammoPerReloadCycle = cfg.maxAmmoCapacity; // A full mag is loaded per reload cycle
            cfg.textureFileName = "pickup_smg.png";
            cfg.rumbleIntensity = 0.4f;
            cfg.rumbleDurationMs = 96;
            cfg.chamberNeedsManualFill = false;
            cfg.chamberManualFillPostFireDelayMillis = 0;
            break;
            
        default:
            logd("Unknown weapon id: %d", id);
            break;
    }
    return cfg;
}

const std::map<int, WeaponConfig> Weapon::weaponConfigs = 
{
    {WeaponId::WEAPON_PISTOL, createWeaponConfig(WeaponId::WEAPON_PISTOL)},
    {WeaponId::WEAPON_SHOTGUN, createWeaponConfig(WeaponId::WEAPON_SHOTGUN)},
    {WeaponId::WEAPON_SMG, createWeaponConfig(WeaponId::WEAPON_SMG)}
};

Weapon::Weapon(WeaponConfig config, TexturePool& texturePool) : GameEntity(),
    id(config.id), 
    ammoType(config.ammoType),
    projectileType(config.projectileType),
    damagePerProjectile(config.damagePerProjectile), 
    maxAmmoCapacity(config.maxAmmoCapacity),
    hasInfiniteAmmo(config.hasInfiniteAmmo),
    fireRateDelayMillis(config.fireRateDelayMillis),
    reloadTimeMillis(config.reloadTimeMillis),
    automatic(config.automatic),
    projectilesPerShot(config.projectilesPerShot),
    spreadAngle(config.spreadAngle),
    ammoPerReloadCycle(config.ammoPerReloadCycle),
    rumbleIntensity(config.rumbleIntensity),
    rumbleDurationMs(config.rumbleDurationMs),
    chamberNeedsManualFill(config.chamberNeedsManualFill),
    chamberManualFillPostFireDelayMillis(config.chamberManualFillPostFireDelayMillis)
{
    if (projectilesPerShot > 1 && spreadAngle > 0)
        angleBetweenProjectiles = (2 * spreadAngle) / (projectilesPerShot - 1);
    texturePath = "res/game_entity/" + config.textureFileName;
    load(texturePool);
}

Weapon::~Weapon()
{
}

void Weapon::update(Level& level)
{
    if (ownerPosition != nullptr)
    {
        rotationOffset.x = offset.x * sin(rotation * M_PI / 180.0f);
        rotationOffset.y = offset.y * cos(rotation * M_PI / 180.0f);

        setPosition(*ownerPosition + *ownerCenter + rotationOffset);
    }

    if (ownerRotation != nullptr)
        setRotation(*ownerRotation - 90);

    autoReloadIfNeeded();
    updateReloadState();
    fillChamberIfNeeded();
}

void Weapon::onFireRequest(const PressedActionData& pressedActionData, 
    std::function<void(const Vector2D&, ProjectileType, int, float)> projectileCreationCallback)
{
    if (isReloading())
        return;
    if (!fireDebouncer.canPerformAction())
        return;
    if (ammoInWeapon == 0)
    {
        switch (id)
        {
        case WeaponId::WEAPON_PISTOL:
            Game::audioManager.playSound(AudioSFXId::PISTOL_DRY_FIRE);
            break;

        case WeaponId::WEAPON_SHOTGUN:
            Game::audioManager.playSound(AudioSFXId::SHOTGUN_DRY_FIRE);
            break;

        case WeaponId::WEAPON_SMG:
            Game::audioManager.playSound(AudioSFXId::SMG_DRY_FIRE);
            break;

        default:
            break;
        }
        return;
    }
    if (chamberNeedsManualFill && !chamberFilledManually)
        return;
    for (int i = 0; i < projectilesPerShot; i++)
    {
        float spread = 0.0f;
        if (spreadAngle > 0 && projectilesPerShot > 1)
        {
            spread = angleBetweenProjectiles * i - spreadAngle;
        }
        if (!automatic)
            Game::control.releaseAssociatedActionsAndHandleActionTriggerBlockedStatus(pressedActionData);
        projectileCreationCallback(position, projectileType, damagePerProjectile, rotation + spread);
    }
    ammoInWeapon--;
    Game::control.rumbleCurrentControllerIfActive({RUMBLE_TRIGGER, 0.0f, rumbleIntensity, rumbleDurationMs});
    lastFireTime = std::chrono::steady_clock::now();
    chamberFilledManually = false;
    switch (id)
    {
    case WeaponId::WEAPON_PISTOL:
        Game::audioManager.playSound(AudioSFXId::PISTOL_FIRE);
        break;

    case WeaponId::WEAPON_SHOTGUN:
        Game::audioManager.playSound(AudioSFXId::SHOTGUN_FIRE);
        break;

    case WeaponId::WEAPON_SMG:
        Game::audioManager.playSound(AudioSFXId::SMG_FIRE);
        break;

    default:
        break;
    }
}

Weapon* Weapon::createWeapon(WeaponId id, TexturePool& texturePool)
{
    auto config = weaponConfigs.find(id);
    if (config == weaponConfigs.end())
        return nullptr;
    return new Weapon(config->second, texturePool);
}

bool Weapon::isOutOfAmmo()
{
    return ammoInWeapon == 0 && ammoInInventory == 0 && !hasInfiniteAmmo;
}

void Weapon::autoReloadIfNeeded()
{
    if (ammoInWeapon == 0)
        reloadIfPossible();
}

void Weapon::reloadIfPossible()
{
    if (ammoInWeapon == maxAmmoCapacity || isReloading() || Game::control.isActionDown(CA_GAME_FIRE))
        return;
    bool hasAvailableAmmo = hasInfiniteAmmo || ammoInInventory >= ammoPerReloadCycle;
    if (!hasAvailableAmmo)
        return;

    reloadAnimator = new FloatAnimator(reloadProgress, 1.0f, reloadTimeMillis);
    switch (id)
    {
    case WeaponId::WEAPON_PISTOL:
        Game::audioManager.playSound(AudioSFXId::PISTOL_RELOAD);
        break;

    case WeaponId::WEAPON_SHOTGUN:
        Game::audioManager.playSound(AudioSFXId::SHOTGUN_RELOAD);
        break;

    case WeaponId::WEAPON_SMG:
        Game::audioManager.playSound(AudioSFXId::SMG_RELOAD);
        break;

    default:
        break;
    }
}

void Weapon::updateReloadState()
{
    if (!isReloading())
        return;
    reloadAnimator->update();
    if (reloadAnimator->isComplete())
    {
        delete reloadAnimator;
        reloadAnimator = nullptr;
        reloadProgress = 0.0f;
        ammoInWeapon = std::min(ammoInWeapon + ammoPerReloadCycle, maxAmmoCapacity);
        ammoInInventory -= ammoPerReloadCycle;
        if (ammoInWeapon < maxAmmoCapacity)
        {
            reloadIfPossible();
        }
    }
}

void Weapon::fillChamberIfNeeded()
{
    if (!chamberNeedsManualFill || ammoInWeapon == 0 || isReloading() || chamberFilledManually)
        return;
    if (std::chrono::steady_clock::now() - lastFireTime > std::chrono::milliseconds(chamberManualFillPostFireDelayMillis))
    {
        chamberFilledManually = true;
        if (id == WeaponId::WEAPON_SHOTGUN)
            Game::audioManager.playSound(AudioSFXId::SHOTGUN_PUMP);
    }
}

void Weapon::setOwner(GameEntity& owner)
{
    ownerPosition = &owner.position;
    ownerWidth = &owner.width;
    ownerHeight = &owner.height;
    ownerRotation = &owner.rotation;
    ownerCenter = &owner.center;
}

