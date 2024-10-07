#include "TileLayer.hpp"

#include "Tile.hpp"
#include "Camera.hpp"
#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

void TileLayer::render(Camera& camera)
{
    for (int y = 0; y < verticalTilesCount; y++)
    {
        for (int x = 0; x < horizontalTilesCount; x++)
        {
            Tile* tile = tileMap[y][x];
            if (tile != nullptr)
            {
                int drawX = (x * GSCALE(tile->width)) - camera.position.getX();
                int drawY = (y * GSCALE(tile->height)) - camera.position.getY();
                tile->draw(drawX, drawY);
            }
        }
    }

    #ifdef DEBUG_DRAW_COLLISION_RECTS
    // Collided tiles: Red
    for (const auto& collidedTileCoords : collidedTiles)
    {
        Tile* tile = tileMap[collidedTileCoords.second][collidedTileCoords.first];
        int drawX = (collidedTileCoords.first * GSCALE(tile->width)) - camera.position.getX();
        int drawY = (collidedTileCoords.second * GSCALE(tile->height)) - camera.position.getY();
        int tileWidth = GSCALE(tile->width);
        int tileHeight = GSCALE(tile->height);
        SDL_Rect tileRectDst = { drawX, drawY, tileWidth, tileHeight };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, tileRectDst, {255, 0, 0, 255}, 2);
    }
    collidedTiles.clear();

    // Check area: Green
    for (const auto& checkAreaRect : checkAreaRects)
    {
        SDL_Rect checkAreaRectDst = 
        {
        static_cast<int>(GSCALE(checkAreaRect.x) - camera.position.getX()),
        static_cast<int>(GSCALE(checkAreaRect.y) - camera.position.getY()),
        static_cast<int>(GSCALE(checkAreaRect.w)),
        static_cast<int>(GSCALE(checkAreaRect.h))
        };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaRectDst, {0, 255, 0, 255}, 1);
    }
    checkAreaRects.clear();

    // Tiles in the check area: Yellow
    for (const auto& checkAreaTileIndicesRect : checkAreaTileIndicesRects)
    {
        SDL_Rect checkAreaTileIndicesDst = 
        {
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.x) - camera.position.getX()),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.y) - camera.position.getY()),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.w)),
            static_cast<int>(GSCALE(checkAreaTileIndicesRect.h))
        };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaTileIndicesDst, {255, 255, 0, 255}, 2);
    }
    checkAreaTileIndicesRects.clear();
    #endif
}
