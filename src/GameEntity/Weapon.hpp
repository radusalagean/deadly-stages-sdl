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

struct WeaponConfig
{
    int id = WeaponId::WEAPON_UNDEFINED;
    int damagePerBullet = 0;
    int bulletsPerMag = 0;
    bool hasInfiniteMags = false;
    int fireRateDelayMillis = 0;
    int reloadTimeMillis = 0;
    bool automatic = false;
    int bulletsPerShot = 0;
    int spreadAngle = 0;
    std::string textureFileName = "";
};

class Weapon : public GameEntity
{
public:
    static const std::map<int, WeaponConfig> weaponConfigs;
    static WeaponConfig createWeaponConfig(int id);
    static Weapon* createWeapon(int id, TexturePool& texturePool);

    Weapon(WeaponConfig config, TexturePool& texturePool);
    ~Weapon();

    void update(Level& level);

    // Config
    int id;
    int damagePerBullet;
    int bulletsPerMag;
    bool hasInfiniteMags;
    int fireRateDelayMillis;
    int reloadTimeMillis;
    bool automatic;
    int bulletsPerShot;
    int spreadAngle;
    float angleBetweenBullets = 0;

    // Fire & Reload
    Debouncer fireDebouncer = Debouncer(fireRateDelayMillis);
    void onFireRequest(std::function<void(const Vector2D&, float)> bulletCreationCallback);
    float reloadProgress = 0.0f;
    FloatAnimator* reloadAnimator = nullptr;
    int availableMags = 0;
    int ammoInCurrentMag = 0;
    bool isOutOfAmmo();
    void autoReloadIfNeeded();
    void reloadIfPossible();
    inline bool isReloading() { return reloadAnimator != nullptr; }
    void updateReloadState();

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
