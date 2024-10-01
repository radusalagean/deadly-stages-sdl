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
    for (const auto& collidedTileCoords : collidedTiles)
    {
        Tile* tile = tileMap[collidedTileCoords.second][collidedTileCoords.first];
        int drawX = (collidedTileCoords.first * GSCALE(tile->width)) - camera.position.getX();
        int drawY = (collidedTileCoords.second * GSCALE(tile->height)) - camera.position.getY();
        int tileWidth = GSCALE(tile->width);
        int tileHeight = GSCALE(tile->height);
        SDL_Rect rect = { drawX, drawY, tileWidth, tileHeight };
        Game::primitiveShapeHelper.drawRectOutline(Game::renderer, rect, {255, 0, 0, 255}, 2);
    }
    #endif
}
