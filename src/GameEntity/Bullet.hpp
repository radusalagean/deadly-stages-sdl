#ifndef __SRC_GAMEENTITY_BULLET_HPP__
#define __SRC_GAMEENTITY_BULLET_HPP__

#include "GameEntity.hpp"

class Bullet : public GameEntity
{
public:
    Bullet(Vector2D weaponPosition, float rotation, TexturePool& texturePool);
    ~Bullet();

    void update(Camera& camera);
    void draw(Camera& camera);

    int speedPxPerSecond = 600;
};

#endif // __SRC_GAMEENTITY_BULLET_HPP__
