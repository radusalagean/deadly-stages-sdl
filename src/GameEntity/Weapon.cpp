#include "Weapon.hpp"

#include <stdlib.h>
#include "../Core/FloatAnimator.hpp"
#include "../Control/Control.hpp"
#include "../Game.hpp"
#include "../Core/TexturePool.hpp"

WeaponConfig Weapon::createWeaponConfig(WeaponId id)
{
    WeaponConfig cfg;
    cfg.id = id;
    switch (id)
    {
        case WeaponId::WEAPON_PISTOL:
            cfg.damagePerBullet = 1;
            cfg.bulletsPerMag = 8;
            cfg.hasInfiniteMags = true;
            cfg.fireRateDelayMillis = 50;
            cfg.reloadTimeMillis = 2000;
            cfg.automatic = false;
            cfg.bulletsPerShot = 1;
            cfg.spreadAngle = 0;
            cfg.textureFileName = "pickup_pistol.png";
            break;
        case WeaponId::WEAPON_SHOTGUN:
            cfg.damagePerBullet = 3;
            cfg.bulletsPerMag = 3;
            cfg.hasInfiniteMags = false;
            cfg.fireRateDelayMillis = 0;
            cfg.reloadTimeMillis = 3000;
            cfg.automatic = false;
            cfg.bulletsPerShot = 3;
            cfg.spreadAngle = 15;
            cfg.textureFileName = "pickup_shotgun.png";
            break;
        case WeaponId::WEAPON_SMG:
            cfg.damagePerBullet = 1;
            cfg.bulletsPerMag = 30;
            cfg.hasInfiniteMags = false;
            cfg.fireRateDelayMillis = 50;
            cfg.reloadTimeMillis = 2000;
            cfg.automatic = true;
            cfg.bulletsPerShot = 1;
            cfg.spreadAngle = 0;
            cfg.textureFileName = "pickup_smg.png";
            break;
    }
    return cfg;
}

const std::map<WeaponId, WeaponConfig> Weapon::weaponConfigs = 
{
    {WeaponId::WEAPON_PISTOL, createWeaponConfig(WeaponId::WEAPON_PISTOL)},
    {WeaponId::WEAPON_SHOTGUN, createWeaponConfig(WeaponId::WEAPON_SHOTGUN)},
    {WeaponId::WEAPON_SMG, createWeaponConfig(WeaponId::WEAPON_SMG)}
};

Weapon::Weapon(WeaponConfig config, TexturePool& texturePool)
    : id(config.id), 
    damagePerBullet(config.damagePerBullet), 
    bulletsPerMag(config.bulletsPerMag),
    hasInfiniteMags(config.hasInfiniteMags),
    fireRateDelayMillis(config.fireRateDelayMillis),
    reloadTimeMillis(config.reloadTimeMillis),
    automatic(config.automatic),
    bulletsPerShot(config.bulletsPerShot),
    spreadAngle(config.spreadAngle)
{
    if (bulletsPerShot > 1 && spreadAngle > 0)
        angleBetweenBullets = (2 * spreadAngle) / (bulletsPerShot - 1);
    texturePath = "res/image/" + config.textureFileName;
    load(texturePool);
}

Weapon::~Weapon()
{
}

void Weapon::update(Level& level)
{
    if (ownerPosition != nullptr)
    {
        float offsetX = offset.x * sin(rotation * M_PI / 180.0f);
        float offsetY = offset.y * cos(rotation * M_PI / 180.0f);

        position = *ownerPosition + *ownerCenter + Vector2D(offsetX, offsetY);
    }

    if (ownerRotation != nullptr)
        rotation = *ownerRotation - 90;

    autoReloadIfNeeded();
    updateReloadState();
}

void Weapon::onFireRequest(std::function<void(const Vector2D&, float)> bulletCreationCallback)
{
    if (isReloading())
        return;
    if (ammoInCurrentMag == 0)
        return;
    if (!fireDebouncer.canPerformAction())
        return;

    for (int i = 0; i < bulletsPerShot; i++)
    {
        if (ammoInCurrentMag == 0)
            break;
        float spread = 0.0f;
        if (spreadAngle > 0 && bulletsPerShot > 1)
        {
            spread = angleBetweenBullets * i - spreadAngle;
        }
        if (!automatic)
            Game::control.releaseAndBlockAction(CA_FIRE);
        bulletCreationCallback(position, rotation + spread);
        ammoInCurrentMag--;
    }
}

Weapon* Weapon::createWeapon(WeaponId id, TexturePool& texturePool)
{
    auto config = weaponConfigs.find(id);
    if (config == weaponConfigs.end())
        return nullptr;
    return new Weapon(config->second, texturePool);
}

void Weapon::autoReloadIfNeeded()
{
    if (ammoInCurrentMag == 0)
        reloadIfPossible();
}

void Weapon::reloadIfPossible()
{
    if (ammoInCurrentMag == bulletsPerMag || isReloading() || Game::control.isActionDown(CA_FIRE))
        return;
    bool hasAvailableMags = hasInfiniteMags || availableMags > 0;
    if (!hasAvailableMags)
        return;

    reloadAnimator = new FloatAnimator(reloadProgress, 1.0f, reloadTimeMillis);
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
        ammoInCurrentMag = bulletsPerMag;
        availableMags--;
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

