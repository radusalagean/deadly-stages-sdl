#include "CollisionManager.hpp"

#include <SDL.h>
#include "../Level/Tile.hpp"
#include "../Level/TileLayer.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "../GameEntity/Enemy.hpp"
#include "../GameEntity/Player.hpp"
#include "../GameEntity/Projectile.hpp"
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
                    intersectionNormal->x = 1;
                    intersectionNormal->y = 0;
                }
                else
                {
                    intersectionNormal->x = -1;
                    intersectionNormal->y = 0;
                }
            }
            else if (tNear.x < tNear.y)
            {
                if (invRayDirection.y < 0)
                {
                    intersectionNormal->x = 0;
                    intersectionNormal->y = 1;
                }
                else
                {
                    intersectionNormal->x = 0;
                    intersectionNormal->y = -1;
                }
            }
            else
            {
                intersectionNormal->x = (rayDirection.x < 0) ? 1 : -1;
                intersectionNormal->y = (rayDirection.y < 0) ? 1 : -1;
            }
        }

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
        float tHitNear = 0.0f;
        float tHitFar = 0.0f;
        return dynamicRectVsRect(dynamicRect, proposedVelocity, staticRect, nullptr, nullptr, tHitNear, tHitFar, tHitNear);
    }

    bool resolveDynamicRectVsRect(const SDL_Rect& dynamicRect, Vector2D& proposedVelocity, const SDL_Rect& staticRect)
    {
        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float tHitNear = 0.0f;
        float tHitFar = 0.0f;

        if (dynamicRectVsRect(dynamicRect, proposedVelocity, staticRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear))
        {
            proposedVelocity += intersectionNormal * Vector2D(std::abs(proposedVelocity.x), std::abs(proposedVelocity.y)) * (1 - tHitNear);
            return true;
        }
        return false;
    }

    bool lineVsLine(const Line& line1, const Line& line2, Vector2D& intersectionPoint)
    {
        float x1 = line1.start.x;
        float y1 = line1.start.y;
        float x2 = line1.end.x;
        float y2 = line1.end.y;
        
        float x3 = line2.start.x;
        float y3 = line2.start.y;
        float x4 = line2.end.x;
        float y4 = line2.end.y;

        // Calculate the denominator of the equations
        float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (denominator == 0) // Lines are parallel
        {
            return false;
        }

        float t1_numerator = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
        float t2_numerator = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);

        float t1 = t1_numerator / denominator;
        float t2 = t2_numerator / denominator;

        if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f) {
            // Calculate the intersection point
            intersectionPoint.x = x1 + t1 * (x2 - x1);
            intersectionPoint.y = y1 + t1 * (y2 - y1);
            return true;
        }
        return false;
    }

    bool sweptRectangleVsLine(const SDL_Rect& rect, const SDL_Rect& proposedRect, const Line& targetLine, Vector2D& intersectionPoint, float& contactTime)
    {
        Line topLeft { 
            {static_cast<float>(rect.x), static_cast<float>(rect.y)}, 
            {static_cast<float>(proposedRect.x), static_cast<float>(proposedRect.y)} 
        };
        Line topRight { 
            {static_cast<float>(rect.x + rect.w), static_cast<float>(rect.y)}, 
            {static_cast<float>(proposedRect.x + proposedRect.w), static_cast<float>(proposedRect.y)} 
        };
        Line bottomLeft { 
            {static_cast<float>(rect.x), static_cast<float>(rect.y + rect.h)}, 
            {static_cast<float>(proposedRect.x), static_cast<float>(proposedRect.y + proposedRect.h)} 
        };
        Line bottomRight { 
            {static_cast<float>(rect.x + rect.w), static_cast<float>(rect.y + rect.h)}, 
            {static_cast<float>(proposedRect.x + proposedRect.w), static_cast<float>(proposedRect.y + proposedRect.h)} 
        };

        Vector2D tempIntersection;
        if (lineVsLine(topLeft, targetLine, tempIntersection) ||
            lineVsLine(topRight, targetLine, tempIntersection) ||
            lineVsLine(bottomLeft, targetLine, tempIntersection) ||
            lineVsLine(bottomRight, targetLine, tempIntersection))
        {
            // Calculate contactTime based on the distance to the intersection point
            Vector2D movement = {static_cast<float>(proposedRect.x - rect.x), static_cast<float>(proposedRect.y - rect.y)};
            float movementLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            
            if (movementLength > 0) {
                Vector2D toIntersection = {tempIntersection.x - rect.x, tempIntersection.y - rect.y};
                float intersectionDistance = std::sqrt(toIntersection.x * toIntersection.x + toIntersection.y * toIntersection.y);
                contactTime = std::max(0.0f, std::min(intersectionDistance / movementLength, 1.0f));
            }

            intersectionPoint = tempIntersection;
            return true;
        }
        return false;
    }

    void processMovement(GameEntity& subjectEntity, EntityType subjectEntityType, Vector2D& proposedVelocity, Level& level, 
        bool subjectEntityJumping)
    {
        GameEntity* outFirstCollidedEntity = nullptr;
        EntityType outFirstCollisionEntityType = EntityType::NONE;
        processMovement(subjectEntity, subjectEntityType, proposedVelocity, level, &outFirstCollidedEntity, outFirstCollisionEntityType, subjectEntityJumping);
    }

    void processMovement(GameEntity& subjectEntity, EntityType subjectEntityType, Vector2D& proposedVelocity, Level& level, 
        GameEntity** outFirstCollidedEntity, EntityType& outFirstCollisionEntityType,
        bool subjectEntityJumping)
    {
        SDL_Rect& subjectBoundsRect = subjectEntity.positionPlusCollisionRect;
        Camera& camera = level.camera;

        int proposedVelocityX = (proposedVelocity.x > 0.0f) ? 
            static_cast<int>(std::ceil(proposedVelocity.x)) : 
            static_cast<int>(std::floor(proposedVelocity.x));

        int proposedVelocityY = (proposedVelocity.y > 0.0f) ? 
            static_cast<int>(std::ceil(proposedVelocity.y)) : 
            static_cast<int>(std::floor(proposedVelocity.y));

        SDL_Rect proposedRect = 
        {
            subjectBoundsRect.x + proposedVelocityX,
            subjectBoundsRect.y + proposedVelocityY,
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

        #ifdef COLLISION_DETECTION_FAST
        if (subjectEntityType != EntityType::PROJECTILE)
        {
            // Step 1: Use fast check to see if there are any collisions
            // Step 1a: Check collisions with tiles
            bool foundCollision = false;
            
            for (int y = startY; y <= endY; ++y) 
            {
                for (int x = startX; x <= endX; ++x) 
                {
                    Tile* tile = level.tileLayer.tileMap[y][x];
                    if (tile != nullptr && tile->isCollidable()) 
                    {
                        SDL_Rect tileRect = level.buildTileRect(x, y);
                        if (rectVsRect(proposedRect, tileRect))
                        {
                            // Fast collision solving when enemy is out of camera view
                            if (subjectEntityType == EntityType::ENEMY && 
                                !camera.isDstRectVisible(subjectEntity.dstRect))
                            {
                                proposedVelocity.reset();
                                outFirstCollisionEntityType = EntityType::TILE;
                                return;
                            }
                            foundCollision = true;
                            break;
                        }
                    }
                }
                if (foundCollision)
                    break;
            }

            if (!foundCollision)
            {
                // Step 1b: Check collision with player
                Player* player = level.player;
                if (&subjectEntity != player)
                {
                    SDL_Rect& playerRect = player->positionPlusCollisionRect;
                    if (rectVsRect(proposedRect, playerRect))
                    {
                        foundCollision = true;
                    }
                }
            }

            if (!foundCollision && !subjectEntityJumping)
            {
                // Step 1c: Check collision with enemies
                for (Enemy* enemy : level.enemies)
                {
                    if (&subjectEntity == enemy)
                        continue;
                    if (!rectVsRect(checkAreaRect, enemy->positionPlusCollisionRect))
                        continue;
                    if (rectVsRect(proposedRect, enemy->positionPlusCollisionRect))
                    {
                        // Fast collision solving when enemy is out of camera view
                        if (subjectEntityType == EntityType::ENEMY && 
                            !camera.isDstRectVisible(subjectEntity.dstRect))
                        {
                            proposedVelocity.reset();
                            *outFirstCollidedEntity = enemy;
                            outFirstCollisionEntityType = EntityType::ENEMY;
                            return;
                        }
                        foundCollision = true;
                        break;
                    }
                }
            }

            if (!foundCollision)
            {
                return;
            }
        }
        #endif // COLLISION_DETECTION_FAST

        // Step 2: Accurate collision detection
        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float tHitNear = 0.0f;
        float tHitFar = 0.0f;
        std::vector<CollisionInfo> collisions;

        // Check only the relevant tiles
        for (int y = startY; y <= endY; ++y) 
        {
            for (int x = startX; x <= endX; ++x) 
            {
                Tile* tile = level.tileLayer.tileMap[y][x];
                if (tile != nullptr && tile->isCollidable()) 
                {
                    SDL_Rect tileRect = level.buildTileRect(x, y);
                    if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, tileRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear)) 
                    {
                        CollisionInfo collision = {tileRect, tHitNear, nullptr, EntityType::TILE};
                        #ifdef DEBUG_DRAW_COLLISION_RECTS
                        level.collidedRects.push_back(tileRect);
                        #endif
                        collisions.push_back(collision);
                    }
                }
            }
        }

        // Check collisions with player
        Player* player = level.player;
        if (&subjectEntity != player)
        {
            SDL_Rect& playerRect = player->positionPlusCollisionRect;
            if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, playerRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, tHitNear))
            {
                CollisionInfo collision = {playerRect, tHitNear, player, EntityType::PLAYER};
                #ifdef DEBUG_DRAW_COLLISION_RECTS
                level.collidedRects.push_back(playerRect);
                #endif
                collisions.push_back(collision);
            }
        }

        if (!subjectEntityJumping)
        {
            // Check collisions with enemies
            for (Enemy* enemy : level.enemies)
            {
                if (&subjectEntity == enemy)
                    continue;
                if (!rectVsRect(checkAreaRect, enemy->positionPlusCollisionRect))
                    continue;

                SDL_Rect& enemyRect = enemy->positionPlusCollisionRect;
                if (subjectEntityType == EntityType::PROJECTILE)
                {
                    #ifdef COLLISION_DETECTION_FAST
                    float& contactTime = tHitFar;
                    if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, enemyRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, contactTime))
                    {
                        CollisionInfo collision = {enemyRect, contactTime, enemy, EntityType::ENEMY};
                        #ifdef DEBUG_DRAW_COLLISION_RECTS
                        level.collidedRects.push_back(enemyRect);
                        #endif
                        collisions.push_back(collision);
                    }
                    #else
                    float contactTime = 0.0f;
                    if (sweptRectangleVsLine(subjectBoundsRect, proposedRect, enemy->positionPlusCollisionLine, intersectionPoint, contactTime))
                    {
                        CollisionInfo collision = {enemyRect, contactTime, enemy, EntityType::ENEMY};
                        collisions.push_back(collision);
                    }
                    #endif
                }
                else
                {
                    float& contactTime = tHitNear;
                    if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, enemyRect, &intersectionPoint, &intersectionNormal, tHitNear, tHitFar, contactTime))
                    {
                        CollisionInfo collision = {enemyRect, contactTime, enemy, EntityType::ENEMY};
                        #ifdef DEBUG_DRAW_COLLISION_RECTS
                        level.collidedRects.push_back(enemyRect);
                        #endif
                        collisions.push_back(collision);
                    }
                }
            }
        }
        
        // Sort intersections by contact time
        std::sort(collisions.begin(), collisions.end(), [](const CollisionInfo& a, const CollisionInfo& b) {
            return a.contactTime < b.contactTime;
        });

        // Resolve collisions
        bool firstCollision = true;
        for (const auto& collision : collisions)
        {
            if (firstCollision)
            {
                if (outFirstCollidedEntity != nullptr)
                {
                    *outFirstCollidedEntity = collision.collidedEntity;
                    outFirstCollisionEntityType = collision.collidedEntityType;
                }
                firstCollision = false;
            }
            
            if (subjectEntityType == EntityType::PROJECTILE)
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