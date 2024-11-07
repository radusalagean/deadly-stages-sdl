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
    
    const float scaledTileWidth = camera.scale(level.tileWidthPx);
    const float scaledTileHeight = camera.scale(level.tileHeightPx);
    
    const int startX = std::max(0, static_cast<int>(camera.position.x / scaledTileWidth));
    const int startY = std::max(0, static_cast<int>(camera.position.y / scaledTileHeight));
    const int endX = std::min(horizontalTilesCount, static_cast<int>((camera.position.x + Game::width) / scaledTileWidth) + 1);
    const int endY = std::min(verticalTilesCount, static_cast<int>((camera.position.y + Game::height) / scaledTileHeight) + 1);
    
    const float camX = camera.position.x;
    const float camY = camera.position.y;
    const bool drawNonCollidable = (renderFlags & RENDER_FLAG_NON_COLLIDABLE_TILES) != 0;
    const bool drawCollidable = (renderFlags & RENDER_FLAG_COLLIDABLE_TILES) != 0;

    for (int y = startY; y < endY; y++)
    {
        const float baseY = y * scaledTileHeight - camY;
        const auto& row = tileMap[y];
        
        for (int x = startX; x < endX; x++)
        {
            Tile* tile = row[x];
            if (tile != nullptr)
            {
                const bool isCollidable = tile->isCollidable();
                if ((drawNonCollidable && !isCollidable) || (drawCollidable && isCollidable))
                {
                    const float drawX = x * scaledTileWidth - camX;
                    tile->draw(camera, drawX, baseY, scaledTileWidth, scaledTileHeight);
                }
            }
        }
    }
}
