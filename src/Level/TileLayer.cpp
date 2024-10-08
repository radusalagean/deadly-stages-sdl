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
                int drawX = (x * GSCALE(tile->width)) - camera.position.x;
                int drawY = (y * GSCALE(tile->height)) - camera.position.y;
                tile->draw(drawX, drawY);
            }
        }
    }
}
