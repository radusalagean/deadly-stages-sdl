#ifndef __SRC_CORE_COLLISIONMANAGER_HPP__
#define __SRC_CORE_COLLISIONMANAGER_HPP__

#include "Vector2D.hpp"
#include "../Level/Level.hpp"

class GameEntity;

namespace CollisionManager
{
    enum class CollisionResolution
    {
        COLLISION_RESOLUTION_PUSH = 1,
        COLLISION_RESOLUTION_VANISH
    };

    struct CollisionInfo
    {
        SDL_Rect collidedRect;
        float contactTime;
        GameEntity* collidedEntity = nullptr;
    };

    bool rectVsRect(const SDL_Rect& rectA, const SDL_Rect& rectB);
    bool rayVsRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const SDL_Rect& targetRect, 
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& tHitNear);
    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect,
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& contactTime);
    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect);
    bool resolveDynamicRectVsRect(const SDL_Rect& dynamicRect, Vector2D& proposedVelocity, const SDL_Rect& staticRect);
    void processMovement(GameEntity& subjectEntity, Vector2D& proposedVelocity, Level& level, GameEntity** firstCollidedEntity = nullptr);
}

#endif // __SRC_CORE_COLLISIONMANAGER_HPP__
