#include "TileLayer.hpp"

#include "Tile.hpp"
#include "Camera.hpp"

void TileLayer::render(Camera& camera)
{
    for (int y = 0; y < verticalTilesCount; y++)
    {
        for (int x = 0; x < horizontalTilesCount; x++)
        {
            Tile* tile = tileMap[y][x];
            if (tile != nullptr)
            {
                int drawX = (x * tile->getScaledWidth()) - camera.position.getX();
                int drawY = (y * tile->getScaledHeight()) - camera.position.getY();
                tile->draw(drawX, drawY);
            }
        }
    }
}