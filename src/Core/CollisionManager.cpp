#include "CollisionManager.hpp"

#include <SDL.h>
#include "../Level/Tile.hpp"
#include "../Level/TileLayer.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../GameEntity/Enemy.hpp"
#include <sstream>
#include <algorithm>
#include <functional>
#include <vector>
#include <utility>
#include <cmath>
#include "Config.hpp"

// Guide: https://youtu.be/8JJ-4JgR7Dg?si=SL0lkRx7hiA5B_GT
namespace CollisionManager
{

    bool rectVsRect(const SDL_Rect& rectA, const SDL_Rect& rectB)
    {
        return SDL_HasIntersection(&rectA, &rectB);
    }

    bool rayVsRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const SDL_Rect& targetRect, 
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& tHitNear, float& tHitFar)
    {
        if (intersectionPoint != nullptr)
        {
            intersectionPoint->reset();
        }
        if (intersectionNormal != nullptr)
        {
            intersectionNormal->reset();
        }

        Vector2D invRayDirection = rayDirection.getInverse();

        Vector2D targetPos = Vector2D(targetRect.x, targetRect.y);
        Vector2D targetSize = Vector2D(targetRect.w, targetRect.h);

        // Calculate the entry and exit distances
        Vector2D tNear = (targetPos - rayOrigin) * invRayDirection;
        Vector2D tFar = (targetPos + targetSize - rayOrigin) * invRayDirection;

        if (std::isnan(tFar.x) || std::isnan(tFar.y)) return false;
        if (std::isnan(tNear.x) || std::isnan(tNear.y)) return false;

        // Sort the entry and exit distances
        if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
        if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

        if (tNear.x > tFar.y || tNear.y > tFar.x)
        {
            return false; // No intersection
        }

        tHitNear = std::max(tNear.x, tNear.y);
        tHitFar = std::min(tFar.x, tFar.y);
        
        if (tHitFar < 0)
        {
            return false; // Ray direction is opposite of the target
        }

        if (intersectionPoint != nullptr)
        {
            *intersectionPoint = rayOrigin + rayDirection * tHitNear;
        }

        if (intersectionNormal != nullptr)
        {
            if (tNear.x > tNear.y)
            {
                if (rayDirection.x < 0)
                {
                    *intersectionNormal = Vector2D(1, 0);
                }
                else
                {
                    *intersectionNormal = Vector2D(-1, 0);
                }
            }
            else if (tNear.x < tNear.y)
            {
                if (invRayDirection.y < 0)
                {
                    *intersectionNormal = Vector2D(0, 1);
                }
                else
                {
                    *intersectionNormal = Vector2D(0, -1);
                }
            }
        }
        
        // Note if tNear == tFar, collision is in a diagonal.
        // By returning a intersectionNormal={0,0} even though its
        // considered a hit, the resolver wont change anything.

        return true;
    }

    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect,
        Vector2D* intersectionPoint, Vector2D* intersectionNormal, float& tHitNear, float& tHitFar, float& contactTime)
    {
        if (proposedVelocity.x == 0 && proposedVelocity.y == 0)
        {
            return false;
        }

        // Expand target rectangle by source dimensions (Explained @ https://youtu.be/8JJ-4JgR7Dg?si=SL0lkRx7hiA5B_GT&t=2013)
        SDL_Rect expandedRect = 
        {
            staticRect.x - dynamicRect.w / 2,
            staticRect.y - dynamicRect.h / 2,
            staticRect.w + dynamicRect.w,
            staticRect.h + dynamicRect.h
        };
        
        Vector2D rayOrigin = Vector2D(dynamicRect.x + dynamicRect.w / 2, dynamicRect.y + dynamicRect.h / 2);

        if (rayVsRect(rayOrigin, proposedVelocity, expandedRect, intersectionPoint, intersectionNormal, tHitNear, tHitFar))
        {
            return contactTime >= 0 && contactTime < 1.0f;
        }
        return false;
    }

    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect)
    {
        float tHitNear = 0;
        float tHitFar = 0;
        return dynamicRectVsRect(dynamicRect, proposedVelocity, staticRect, nullptr, nullptr, tHitNear, tHitFar, tHitNear);
    }

    bool resolveDynamicRectVsRect(const SDL_Rect& dynamicRect, Vector2D& proposedVelocity, const SDL_Rect& staticRect)
    {
        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float tHitNear = 0;
        float tHitFar = 0;

        if (dynamicRectVsRect(dynamicRect, proposedVelocity, staticRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear))
        {
            proposedVelocity += intersectionNormal * Vector2D(std::abs(proposedVelocity.x), std::abs(proposedVelocity.y)) * (1 - tHitNear);
            return true;
        }
        return false;
    }

    void processMovement(GameEntity& subjectEntity, Vector2D& proposedVelocity, Level& level, GameEntity** firstCollidedEntity)
    {
        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float tHitNear = 0;
        float tHitFar = 0;
        std::vector<CollisionInfo> collisions;

        bool isSubjectEntityBullet = dynamic_cast<Bullet*>(&subjectEntity);

        SDL_Rect subjectBoundsRect = 
        {
            static_cast<int>(subjectEntity.position.x) + subjectEntity.collisionRect.x,
            static_cast<int>(subjectEntity.position.y) + subjectEntity.collisionRect.y,
            subjectEntity.collisionRect.w,
            subjectEntity.collisionRect.h
        };
        SDL_Rect proposedRect = 
        {
            subjectBoundsRect.x + static_cast<int>(proposedVelocity.x),
            subjectBoundsRect.y + static_cast<int>(proposedVelocity.y),
            subjectBoundsRect.w,
            subjectBoundsRect.h
        };

        SDL_Rect checkAreaRect = Game::primitiveShapeHelper.wrapRects(subjectBoundsRect, proposedRect);

        #ifdef DEBUG_DRAW_COLLISION_RECTS
        level.checkAreaRects.push_back(checkAreaRect);
        #endif

        // Calculate the tile indices for the check area rectangle
        int startX = (checkAreaRect.x / level.tileWidthPx) - 1;
        int startY = (checkAreaRect.y / level.tileHeightPx) - 1;
        int endX = (checkAreaRect.x + checkAreaRect.w) / level.tileWidthPx + 1;
        int endY = (checkAreaRect.y + checkAreaRect.h) / level.tileHeightPx + 1;
        
        // Clamp indices to level bounds
        startX = std::max(0, startX);
        startY = std::max(0, startY);
        endX = std::min(level.horizontalTilesCount - 1, endX);
        endY = std::min(level.verticalTilesCount - 1, endY);

        #ifdef DEBUG_DRAW_COLLISION_RECTS
        level.checkAreaTileIndicesRects.push_back(
            {
                level.tileWidthPx * startX, 
                level.tileHeightPx * startY, 
                level.tileWidthPx * (endX - startX + 1), 
                level.tileHeightPx * (endY - startY + 1)
            }
        );
        #endif

        // Check only the relevant tiles
        for (int y = startY; y <= endY; ++y) 
        {
            for (int x = startX; x <= endX; ++x) 
            {
                Tile* tile = level.tileLayer.tileMap[y][x];
                if (tile->isCollidable()) 
                {
                    SDL_Rect tileRect = level.buildTileRect(x, y);
                    if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, tileRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear)) 
                    {
                        SDL_Rect collidedRect = level.buildTileRect(x, y);
                        CollisionInfo collision = {collidedRect, tHitNear};
                        #ifdef DEBUG_DRAW_COLLISION_RECTS
                        level.collidedRects.push_back(collidedRect);
                        #endif
                        collisions.push_back(collision);
                    }
                }
            }
        }

        // Check collisions with player
        Player* player = level.player;
        if (&subjectEntity != player) // TODO Check only in a restricted area
        {
            SDL_Rect playerRect = 
            {
                static_cast<int>(player->position.x) + player->collisionRect.x,
                static_cast<int>(player->position.y) + player->collisionRect.y,
                player->collisionRect.w,
                player->collisionRect.h
            };
            if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, playerRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear))
            {
                CollisionInfo collision = {playerRect, tHitNear, player};
                #ifdef DEBUG_DRAW_COLLISION_RECTS
                level.collidedRects.push_back(playerRect);
                #endif
                collisions.push_back(collision);
            }
        }

        // Check collisions with enemies
        for (Enemy* enemy : level.enemies)
        {
            if (&subjectEntity == enemy)
                continue;
            SDL_Rect enemyRect = 
            {
                static_cast<int>(enemy->position.x) + enemy->collisionRect.x,
                static_cast<int>(enemy->position.y) + enemy->collisionRect.y,
                enemy->collisionRect.w,
                enemy->collisionRect.h
            };
            float& contactTime = isSubjectEntityBullet ? tHitFar : tHitNear;
            if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, enemyRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, contactTime))
            {
                CollisionInfo collision = {enemyRect, contactTime, enemy};
                #ifdef DEBUG_DRAW_COLLISION_RECTS
                level.collidedRects.push_back(enemyRect);
                #endif
                collisions.push_back(collision);
            }
        }

        // Sort intersections by contact time
        std::sort(collisions.begin(), collisions.end(), [](const CollisionInfo& a, const CollisionInfo& b) {
            return a.contactTime < b.contactTime;
        });

        // Resolve collisions
        for (const auto& collision : collisions)
        {
            if (firstCollidedEntity != nullptr && *firstCollidedEntity == nullptr)
            {
                *firstCollidedEntity = collision.collidedEntity;
            }
            if (isSubjectEntityBullet)
            {
                subjectEntity.pendingRemoval = true;
                proposedVelocity.reset();
                break;
            }
            else
            {
                resolveDynamicRectVsRect(subjectBoundsRect, proposedVelocity, collision.collidedRect);
            }
        }
    }
}