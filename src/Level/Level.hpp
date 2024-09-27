#ifndef __SRC_LEVEL_LEVEL_HPP_
#define __SRC_LEVEL_LEVEL_HPP_

#include <map>
#include "Tile.hpp"
#include "TileLayer.hpp"
#include "../Core/TexturePool.hpp"
#include "../GameEntity/Player.hpp"
#include "Camera.hpp"

class Level 
{
public:
    Level();

    void load();
    void handleEvents();
    void update();
    void render();
    void dispose();

    std::map<int, Tile> tileset{};

    TileLayer tileLayer;
    TexturePool texturePool;
    Player* player = nullptr;
    Camera camera;

    int horizontalTilesCount = 0;
    int verticalTilesCount = 0;
    int widthPx = 0;
    int heightPx = 0;
};

#endif // __SRC_LEVEL_LEVEL_HPP_
