#include "TileLayer.hpp"

#include "Tile.hpp"
#include "Camera.hpp"
#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"

void TileLayer::render(Camera& camera, int renderFlags)
{
    int windowWidth = Game::width;
    int windowHeight = Game::height;

    int startX = std::max(0, static_cast<int>(camera.position.x / GSCALE(tileMap[0][0]->width)));
    int startY = std::max(0, static_cast<int>(camera.position.y / GSCALE(tileMap[0][0]->height)));
    int endX = std::min(horizontalTilesCount, static_cast<int>((camera.position.x + windowWidth) / GSCALE(tileMap[0][0]->width)) + 1);
    int endY = std::min(verticalTilesCount, static_cast<int>((camera.position.y + windowHeight) / GSCALE(tileMap[0][0]->height)) + 1);

    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            Tile* tile = tileMap[y][x];
            if (tile != nullptr)
            {
                bool canDraw = ((renderFlags & RENDER_FLAG_NON_COLLIDABLE_TILES && !tile->isCollidable())
                             || (renderFlags & RENDER_FLAG_COLLIDABLE_TILES && tile->isCollidable()));
                if (canDraw)
                {
                    int drawX = (x * GSCALE(tile->width)) - camera.position.x;
                    int drawY = (y * GSCALE(tile->height)) - camera.position.y;
                    tile->draw(drawX, drawY);
                }
            }
        }
    }
}
