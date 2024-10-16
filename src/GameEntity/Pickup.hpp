#ifndef __SRC_GAMEENTITY_PICKUP_HPP__
#define __SRC_GAMEENTITY_PICKUP_HPP__

#include "GameEntity.hpp"
#include "../Core/FloatAnimator.hpp"

enum PickupId
{
    PICKUP_UNDEFINED = -1,
    PICKUP_HEALTH,
    PICKUP_SHOTGUN,
    PICKUP_SMG,
    PICKUP_ENUM_COUNT
};

class Pickup : public GameEntity
{
public:
    Pickup(int id, Vector2D position, TexturePool& texturePool);
    ~Pickup();

    void update(Level& level);
    void draw(Camera& camera);

    static constexpr float LIFETIME_SECONDS = 10.0f;
    static constexpr int PICKUP_SIZE = 32;
    static constexpr int FLOAT_ANIMATOR_DURATION_MS = 500;
    static constexpr float MAX_DST_RECT_SCALE = 1.12f;

    float aliveTimeSeconds = 0.0f;
    int id;
    FloatAnimator floatAnimator = FloatAnimator(dstRectScale, MAX_DST_RECT_SCALE, FLOAT_ANIMATOR_DURATION_MS);
    bool isIncreasing = true;
};

#endif // __SRC_GAMEENTITY_PICKUP_HPP__
