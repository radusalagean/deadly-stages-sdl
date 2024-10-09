#ifndef __SRC_GAMEENTITY_WEAPON_HPP__
#define __SRC_GAMEENTITY_WEAPON_HPP__

#include "GameEntity.hpp"
#include "../Core/Debouncer.hpp"

class Weapon : public GameEntity
{
public:
    Weapon(int id, int damagePerBullet, int ammo, bool hasInfiniteAmmo, 
        int fireRateMillis, int bulletsPerShot, int spreadAngle);
    ~Weapon();

    void update(Level& level);
    void onFireRequest(std::function<void(const Vector2D&, float)> bulletCreationCallback);

    int id = 0;
    int damagePerBullet = 0;
    int ammo = 0;
    bool hasInfiniteAmmo = false;
    int fireRateMillis = 0;
    int bulletsPerShot = 0;
    int spreadAngle = 0;
    float angleBetweenBullets = 0;
    
    void setOwner(GameEntity& owner);

    Debouncer fireDebouncer = Debouncer(fireRateMillis);

    static Weapon* createWeapon(int id);

    

private:
    Vector2D* ownerPosition = nullptr;
    int* ownerWidth = nullptr;
    int* ownerHeight = nullptr;
    double* ownerRotation = nullptr;
    Vector2D* ownerCenter = nullptr;
    Vector2D position;
    Vector2D offset = Vector2D(20, -20);
};

enum WeaponId
{
    WEAPON_PISTOL = 1,
    WEAPON_SHOTGUN,
    WEAPON_SMG
};

#endif // __SRC_GAMEENTITY_WEAPON_HPP__
