#ifndef __SRC_LEVEL_TILELAYER_HPP_
#define __SRC_LEVEL_TILELAYER_HPP_

#include <vector>
#include <utility>
#include <SDL.h>
#include "../Core/Config.hpp"

class Camera;
class Tile;

class TileLayer
{
public:
    static const int RENDER_FLAG_NON_COLLIDABLE_TILES = 1 << 0;
    static const int RENDER_FLAG_COLLIDABLE_TILES = 1 << 1;

    int horizontalTilesCount = 0;
    int verticalTilesCount = 0;
    std::vector<std::vector<Tile*>> tileMap{{}};

    void render(Camera& camera, int renderFlags);
};


#endif // __SRC_LEVEL_TILELAYER_HPP_
