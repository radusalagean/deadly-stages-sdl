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

    // Check area: Green
    SDL_Rect checkAreaRectDst = 
    {
        static_cast<int>(GSCALE(checkAreaRect.x) - camera.position.getX()),
        static_cast<int>(GSCALE(checkAreaRect.y) - camera.position.getY()),
        static_cast<int>(GSCALE(checkAreaRect.w)),
        static_cast<int>(GSCALE(checkAreaRect.h))
    };
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaRectDst, {0, 255, 0, 255}, 1);

    // Tiles in the check area: Yellow
    SDL_Rect checkAreaTileIndicesDst = 
    {
        static_cast<int>(GSCALE(checkAreaTileIndices.x) - camera.position.getX()),
        static_cast<int>(GSCALE(checkAreaTileIndices.y) - camera.position.getY()),
        static_cast<int>(GSCALE(checkAreaTileIndices.w)),
        static_cast<int>(GSCALE(checkAreaTileIndices.h))
    };
    Game::primitiveShapeHelper.drawRectOutline(Game::renderer, checkAreaTileIndicesDst, {255, 255, 0, 255}, 2);
    #endif
}
