#ifndef __SRC_GAMEENTITY_WEAPON_HPP__
#define __SRC_GAMEENTITY_WEAPON_HPP__

#include "GameEntity.hpp"
#include "../Core/Debouncer.hpp"
#include <functional>

class FloatAnimator;
class TexturePool;

enum WeaponId
{
    WEAPON_UNDEFINED = -1,
    WEAPON_PISTOL,
    WEAPON_SHOTGUN,
    WEAPON_SMG,
    WEAPON_ENUM_COUNT
};

enum AmmoType // What gets loaded in the weapon // TODO Remove if not needed
{
    AMMO_TYPE_UNDEFINED = -1,
    AMMO_TYPE_9MM_BULLET,
    AMMO_TYPE_00_BUCK_SHELL
};

enum ProjectileType // What gets fired from the weapon
{
    PROJECTILE_TYPE_UNDEFINED = -1,
    PROJECTILE_TYPE_9MM_BULLET,
    PROJECTILE_TYPE_00_BUCK_SHELL_PELLET
};

struct WeaponConfig
{
    WeaponId id = WeaponId::WEAPON_UNDEFINED;
    AmmoType ammoType = AMMO_TYPE_UNDEFINED;
    ProjectileType projectileType = PROJECTILE_TYPE_UNDEFINED;
    int damagePerProjectile = 0;
    int maxAmmoCapacity = 0;
    bool hasInfiniteAmmo = false;
    int fireRateDelayMillis = 0;
    int reloadTimeMillis = 0;
    bool automatic = false;
    int projectilesPerShot = 0;
    int spreadAngle = 0;
    int ammoPerReloadCycle = 0;
    std::string textureFileName = "";
    float rumbleIntensity = 0.0f;
    Uint32 rumbleDurationMs = 0;
    bool chamberNeedsManualFill = false;
    int chamberManualFillPostFireDelayMillis = 0;
};

class Weapon : public GameEntity
{
public:
    static const std::map<int, WeaponConfig> weaponConfigs;
    static WeaponConfig createWeaponConfig(WeaponId id);
    static Weapon* createWeapon(WeaponId id, TexturePool& texturePool);

    Weapon(WeaponConfig config, TexturePool& texturePool);
    ~Weapon();

    void update(Level& level);

    // Config
    WeaponId id;
    AmmoType ammoType;
    ProjectileType projectileType;
    int damagePerProjectile;
    int maxAmmoCapacity;
    bool hasInfiniteAmmo;
    int fireRateDelayMillis;
    int reloadTimeMillis;
    bool automatic;
    int projectilesPerShot;
    int spreadAngle;
    int ammoPerReloadCycle;
    float rumbleIntensity;
    Uint32 rumbleDurationMs;
    bool chamberNeedsManualFill;
    int chamberManualFillPostFireDelayMillis;
    float angleBetweenProjectiles = 0;

    // Fire & Reload
    Debouncer fireDebouncer = Debouncer(fireRateDelayMillis);
    void onFireRequest(const PressedActionData& pressedActionData, 
        std::function<void(const Vector2D&, ProjectileType, int, float)> projectileCreationCallback);
    float reloadProgress = 0.0f;
    FloatAnimator* reloadAnimator = nullptr;
    int ammoInInventory = 0;
    int ammoInWeapon = 0;
    bool chamberFilledManually = false;
    std::chrono::steady_clock::time_point lastFireTime;
    bool isOutOfAmmo();
    void autoReloadIfNeeded();
    void reloadIfPossible();
    inline bool isReloading() { return reloadAnimator != nullptr; }
    void updateReloadState();
    void fillChamberIfNeeded();

    // Owner
    void setOwner(GameEntity& owner);

private:
    Vector2D* ownerPosition = nullptr;
    int* ownerWidth = nullptr;
    int* ownerHeight = nullptr;
    double* ownerRotation = nullptr;
    Vector2D* ownerCenter = nullptr;
    Vector2D offset = Vector2D(20, -20);
};

#endif // __SRC_GAMEENTITY_WEAPON_HPP__
