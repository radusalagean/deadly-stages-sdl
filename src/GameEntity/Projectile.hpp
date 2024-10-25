#ifndef __SRC_GAMEENTITY_PROJECTILE_HPP__
#define __SRC_GAMEENTITY_PROJECTILE_HPP__

#include "GameEntity.hpp"
#include "Weapon.hpp"

class Level;

class Projectile : public GameEntity
{
public:
    Projectile(Vector2D weaponPosition, ProjectileType projectileType, int damage, float rotation, TexturePool& texturePool);
    ~Projectile();

    void update(Level& level);
    void draw(Camera& camera);

    int speedPxPerSecond = 600;
};

#endif // __SRC_GAMEENTITY_PROJECTILE_HPP__
