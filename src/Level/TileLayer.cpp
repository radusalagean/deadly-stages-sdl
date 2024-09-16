#include "TileLayer.hpp"

#include "Tile.hpp"

void TileLayer::render(Camera& camera)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Tile* tile = tileMap[y][x];
            if (tile != nullptr)
            {
                tile->draw(x * tile->getScaledWidth(), y * tile->getScaledHeight());
            }
        }
    }
}