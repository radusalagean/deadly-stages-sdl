#include "Weapon.hpp"

#include <stdlib.h>

Weapon::Weapon(int id, int damagePerBullet, int ammo, bool hasInfiniteAmmo, int fireRateMillis, int bulletsPerShot, int spreadAngle)
    : id(id), damagePerBullet(damagePerBullet), ammo(ammo), hasInfiniteAmmo(hasInfiniteAmmo), 
    fireRateMillis(fireRateMillis), bulletsPerShot(bulletsPerShot), spreadAngle(spreadAngle)
{
    if (bulletsPerShot > 1 && spreadAngle > 0)
        angleBetweenBullets = (2 * spreadAngle) / (bulletsPerShot - 1);
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
}

void Weapon::onFireRequest(std::function<void(const Vector2D&, float)> bulletCreationCallback)
{
    if (!fireDebouncer.canPerformAction())
        return;
    logd("Firing weapon");

    for (int i = 0; i < bulletsPerShot; i++)
    {
        float spread = 0.0f;
        if (spreadAngle > 0 && bulletsPerShot > 1)
        {
            spread = angleBetweenBullets * i - spreadAngle;
        }
        logd("Firing bullet %d with spread %f", i, spread);
        bulletCreationCallback(position, rotation + spread);
    }
}

Weapon* Weapon::createWeapon(int id)
{
    switch (id)
    {
        case WEAPON_PISTOL:
            return new Weapon(WEAPON_PISTOL, 1, 0, true, 750, 1, 0);
        case WEAPON_SHOTGUN:
            return new Weapon(WEAPON_SHOTGUN, 3, 90, false, 1300, 3, 15);
        case WEAPON_SMG:
            return new Weapon(WEAPON_SMG, 1, 240, false, 150, 1, 0);
    }
    return nullptr;
}

void Weapon::setOwner(GameEntity& owner)
{
    ownerPosition = &owner.position;
    ownerWidth = &owner.width;
    ownerHeight = &owner.height;
    ownerRotation = &owner.rotation;
    ownerCenter = &owner.center;
}

