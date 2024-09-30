#include "TileLayer.hpp"

#include "Tile.hpp"
#include "Camera.hpp"
#include "../Core/Macros.hpp"

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

    // Debug collisions
    for (const auto& collidedTileCoords : collidedTiles)
    {
        logd("Collided tile: x=%d, y=%d", collidedTileCoords.first, collidedTileCoords.second);
        Tile* tile = tileMap[collidedTileCoords.second][collidedTileCoords.first];
        int drawX = (collidedTileCoords.first * GSCALE(tile->width)) - camera.position.getX();
        int drawY = (collidedTileCoords.second * GSCALE(tile->height)) - camera.position.getY();
        // Draw a red rectangle over the tile
        int tileWidth = GSCALE(tile->width);
        int tileHeight = GSCALE(tile->height);
        SDL_Rect rect = { drawX, drawY, tileWidth, tileHeight };
        SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(Game::renderer, &rect);
    }
}
