#ifndef __SRC_GAMEENTITY_BULLET_HPP__
#define __SRC_GAMEENTITY_BULLET_HPP__

#include "GameEntity.hpp"

class Level;

class Bullet : public GameEntity
{
public:
    Bullet(Vector2D weaponPosition, float rotation, TexturePool& texturePool);
    ~Bullet();

    void update(Camera& camera, Level& level);
    void draw(Camera& camera);

    int speedPxPerSecond = 6;
    // int speedPxPerSecond = 600;

    CollisionManager::CollisionResolution getCollisionResolution() override;
};

#endif // __SRC_GAMEENTITY_BULLET_HPP__
