#ifndef __SRC_CORE_COLLISIONMANAGER_HPP__
#define __SRC_CORE_COLLISIONMANAGER_HPP__

#include "Vector2D.hpp"
#include "../Level/Level.hpp"
#include "Line.hpp"

class GameEntity;

namespace CollisionManager
{

    enum class EntityType
    {
        NONE,
        TILE,
        PLAYER,
        ENEMY,
        PROJECTILE
    };

    struct CollisionInfo
    {
        SDL_Rect collidedRect;
        float contactTime;
        GameEntity* collidedEntity = nullptr;
        EntityType collidedEntityType = EntityType::NONE;
    };

    bool rectVsRect(const SDL_Rect& rectA, const SDL_Rect& rectB);
    bool rayVsRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const SDL_Rect& targetRect, 
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& tHitNear, float& tHitFar);
    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect,
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& tHitNear, float& tHitFar, float& contactTime);
    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect);
    bool resolveDynamicRectVsRect(const SDL_Rect& dynamicRect, Vector2D& proposedVelocity, const SDL_Rect& staticRect);
    bool lineVsLine(const Line& line1, const Line& line2, Vector2D& intersectionPoint);
    bool sweptRectangleVsLine(const SDL_Rect& rect, const SDL_Rect& proposedRect,
        const Line& targetLine, Vector2D& intersectionPoint, float& contactTime);
    void processMovement(GameEntity& subjectEntity, EntityType subjectEntityType, Vector2D& proposedVelocity, Level& level,
        bool subjectEntityJumping = false);
    void processMovement(GameEntity& subjectEntity, EntityType subjectEntityType, Vector2D& proposedVelocity, Level& level, 
        GameEntity** outFirstCollidedEntity, EntityType& outFirstCollisionEntityType,
        bool subjectEntityJumping = false);
}

#endif // __SRC_CORE_COLLISIONMANAGER_HPP__
