#include "CollisionManager.hpp"

#include <SDL.h>
#include "../Level/Tile.hpp"
#include "../Level/TileLayer.hpp"
#include <sstream>
#include <algorithm>
#include <functional>

// Guide: https://youtu.be/8JJ-4JgR7Dg?si=SL0lkRx7hiA5B_GT
namespace CollisionManager
{

    bool rectVsRect(const SDL_Rect& rectA, const SDL_Rect& rectB)
    {
        return SDL_HasIntersection(&rectA, &rectB);
    }

    bool rayVsRect(const Vector2D& rayOrigin, const Vector2D& rayDirection, const SDL_Rect& targetRect, 
        Vector2D& intersectionPoint, Vector2D& intersectionNormal, float& tHitNear)
    {
        intersectionPoint.reset();
        intersectionNormal.reset();

        Vector2D invRayDirection = rayDirection.getInverse();

        Vector2D targetPos = Vector2D(targetRect.x, targetRect.y);
        Vector2D targetSize = Vector2D(targetRect.w, targetRect.h);

        // Calculate the entry and exit distances
        Vector2D tNear = (targetPos - rayOrigin) * invRayDirection;
        Vector2D tFar = (targetPos + targetSize - rayOrigin) * invRayDirection;

        // Sort the entry and exit distances
        if (tNear.getX() > tFar.getX()) std::swap(tNear.getX(), tFar.getX());
        if (tNear.getY() > tFar.getY()) std::swap(tNear.getY(), tFar.getY());

        if (tNear.getX() > tFar.getY() || tNear.getY() > tFar.getX())
        {
            return false; // No intersection
        }

        tHitNear = std::max(tNear.getX(), tNear.getY());
        float tHitFar = std::min(tFar.getX(), tFar.getY());
        
        if (tHitFar < 0)
        {
            return false; // Ray direction is opposite of the target
        }

        intersectionPoint = rayOrigin + rayDirection * tHitNear;
        if (tNear.getX() > tNear.getY())
        {
            if (rayDirection.getX() < 0)
            {
                intersectionNormal = Vector2D(1, 0);
            }
            else
            {
                intersectionNormal = Vector2D(-1, 0);
            }
        }
        else if (tNear.getX() < tNear.getY())
        {
            if (invRayDirection.getY() < 0)
            {
                intersectionNormal = Vector2D(0, 1);
            }
            else
            {
                intersectionNormal = Vector2D(0, -1);
            }
        }
        
        // Note if tNear == tFar, collision is in a diagonal.
        // By returning a intersectionNormal={0,0} even though its
        // considered a hit, the resolver wont change anything.

        return true;
    }

    bool dynamicRectVsRect(const SDL_Rect& dynamicRect, const Vector2D& proposedVelocity, const SDL_Rect& staticRect,
        Vector2D& intersectionPoint, Vector2D& intersectionNormal, float& contactTime)
    {
        if (proposedVelocity.getX() == 0 && proposedVelocity.getY() == 0)
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

        if (rayVsRect(rayOrigin, proposedVelocity, expandedRect, intersectionPoint, intersectionNormal, contactTime))
        {
            return contactTime >= 0 && contactTime < 1.0f;
        }
        return false;
    }

    bool resolveDynamicRectVsRect(const SDL_Rect& dynamicRect, Vector2D& proposedVelocity, const SDL_Rect& staticRect)
    {
        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float contactTime = 0;

        if (dynamicRectVsRect(dynamicRect, proposedVelocity, staticRect, intersectionPoint, intersectionNormal, contactTime))
        {
            proposedVelocity += intersectionNormal * Vector2D(std::abs(proposedVelocity.getX()), std::abs(proposedVelocity.getY())) * (1 - contactTime);
            return true;
        }
        return false;
    }

    void processMovement(const GameEntity& subjectEntity, Vector2D& proposedVelocity, const Level& level)
    {

        Vector2D intersectionPoint;
        Vector2D intersectionNormal;
        float contactTime = 0;
        float minimumContactTime = INFINITY;
        std::vector<std::pair<Vector2D, float>> sortedIntersections; // Key: Tile coordinates, Value: contactTime

        SDL_Rect subjectBoundsRect = 
        {
            static_cast<int>(subjectEntity.position.getX()) + subjectEntity.collisionRect.x,
            static_cast<int>(subjectEntity.position.getY()) + subjectEntity.collisionRect.y,
            subjectEntity.collisionRect.w,
            subjectEntity.collisionRect.h
        };
        SDL_Rect proposedRect = 
        {
            subjectBoundsRect.x + static_cast<int>(proposedVelocity.getX()),
            subjectBoundsRect.y + static_cast<int>(proposedVelocity.getY()),
            subjectBoundsRect.w,
            subjectBoundsRect.h
        };

        // Calculate the tile indices for the proposed rectangle
        int startX = proposedRect.x / level.tileWidthPx;
        int startY = proposedRect.y / level.tileHeightPx;
        int endX = (proposedRect.x + proposedRect.w) / level.tileWidthPx;
        int endY = (proposedRect.y + proposedRect.h) / level.tileHeightPx;

        // TODO
        startX = 0;
        startY = 0;
        endX = 1000;
        endY = 1000;

        // Clamp indices to level bounds
        startX = std::max(0, startX);
        startY = std::max(0, startY);
        endX = std::min(level.horizontalTilesCount - 1, endX);
        endY = std::min(level.verticalTilesCount - 1, endY);

        std::stringstream ss;
        ss << "Proposed rect: " << proposedRect.x << ", " << proposedRect.y << ", " << proposedRect.w << ", " << proposedRect.h << std::endl;
        ss << "Start: " << startX << ", " << startY << std::endl;
        ss << "End: " << endX << ", " << endY << std::endl;
        logd(ss.str().c_str());

        // Check only the relevant tiles
        for (int y = startY; y <= endY; ++y) 
        {
            for (int x = startX; x <= endX; ++x) 
            {
                Tile* tile = level.tileLayer.tileMap[y][x];
                if (tile->isCollidable()) 
                {
                    SDL_Rect tileRect = level.buildTileRect(x, y);
                    if (dynamicRectVsRect(subjectBoundsRect, proposedVelocity, tileRect, intersectionPoint, intersectionNormal, contactTime)) 
                    {
                        Vector2D tileCoords = Vector2D(x, y);
                        std::pair<Vector2D, float> intersection = std::make_pair(tileCoords, contactTime);
                        sortedIntersections.push_back(intersection);
                    }
                }
            }
        }

        if (!sortedIntersections.empty())
        {
            logd("Found %d intersections", sortedIntersections.size());
            for (const auto& intersection : sortedIntersections) 
            {
                logd("Intersection: %f, %f, %f", intersection.first.getX(), intersection.first.getY(), intersection.second);
            }
            logd("=== End of unsorted intersections ===");
        }

        // Sort intersections by contact time
        std::sort(sortedIntersections.begin(), sortedIntersections.end(), [](const std::pair<Vector2D, float>& a, const std::pair<Vector2D, float>& b) {
            return a.second < b.second;
        });

        if (!sortedIntersections.empty())
        {
            logd("After sorting:");
            for (const auto& intersection : sortedIntersections) 
            {
                logd("Intersection: %f, %f, %f", intersection.first.getX(), intersection.first.getY(), intersection.second);
            }
            logd("=== End of sorted intersections ===");
        }

        // Resolve collisions
        for (const auto& intersection : sortedIntersections) 
        {
            SDL_Rect tileRect = level.buildTileRect(intersection.first.getX(), intersection.first.getY());
            logd("Resolving intersection: x%d, %d, %d, %d", tileRect.x, tileRect.y, tileRect.w, tileRect.h);
            resolveDynamicRectVsRect(subjectBoundsRect, proposedVelocity, tileRect);
        }
    }
}