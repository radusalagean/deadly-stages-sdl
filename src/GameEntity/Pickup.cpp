#include "Pickup.hpp"

Pickup::Pickup(int id, Vector2D position, TexturePool& texturePool) : GameEntity(), id(id)
{
    setSize(PICKUP_SIZE, PICKUP_SIZE);
    setPosition(position);
    switch (id)
    {
        case PICKUP_HEALTH:
            texturePath = "res/image/pickup_hp.png";
            break;
        case PICKUP_SHOTGUN:
            texturePath = "res/image/pickup_shotgun.png";
            break;
        case PICKUP_SMG:
            texturePath = "res/image/pickup_smg.png";
            break;

        default:
            break;
    }
    castShadow = true;
    load(texturePool);
}

Pickup::~Pickup()
{
}

void Pickup::update(Level& level)
{
    GameEntity::update(level);
    floatAnimator.update();
    if (floatAnimator.isComplete())
    {
        isIncreasing = !isIncreasing;
        floatAnimator.setNewTargetValueAndReset(isIncreasing ? MAX_DST_RECT_SCALE : 1.0f);
    }
    if (aliveTimeSeconds > LIFETIME_SECONDS)
    {
        pendingRemoval = true;
    }
    else
    {
        aliveTimeSeconds += Game::latestLoopDeltaTimeSeconds;
    }
}

void Pickup::draw(Camera& camera)
{
    GameEntity::draw(camera);
}
