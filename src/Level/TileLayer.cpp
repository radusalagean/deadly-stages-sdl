#include "TileLayer.hpp"

#include "Tile.hpp"
#include "Camera.hpp"
#include "../Core/Macros.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include "Level.hpp"

void TileLayer::render(Level& level, int renderFlags)
{
    Camera& camera = level.camera;

    const int windowWidth = Game::width;
    const int windowHeight = Game::height;

    const int startX = std::max(0, static_cast<int>(camera.position.x / camera.scale(level.tileWidthPx)));
    const int startY = std::max(0, static_cast<int>(camera.position.y / camera.scale(level.tileHeightPx)));
    const int endX = std::min(horizontalTilesCount, static_cast<int>((camera.position.x + windowWidth) / camera.scale(level.tileWidthPx)) + 1);
    const int endY = std::min(verticalTilesCount, static_cast<int>((camera.position.y + windowHeight) / camera.scale(level.tileHeightPx)) + 1);

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
                    int drawX = x * camera.scale(level.tileWidthPx) - camera.position.x;
                    int drawY = y * camera.scale(level.tileHeightPx) - camera.position.y;
                    tile->draw(camera, drawX, drawY);
                }
            }
        }
    }
}
