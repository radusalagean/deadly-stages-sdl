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
    int horizontalTilesCount = 0;
    int verticalTilesCount = 0;
    std::vector<std::vector<Tile*>> tileMap{{}};
    #ifdef DEBUG_DRAW_COLLISION_RECTS
    std::vector<std::pair<int, int>> collidedTiles{};
    std::vector<SDL_Rect> checkAreaRects;
    std::vector<SDL_Rect> checkAreaTileIndicesRects;
    #endif

    void render(Camera& camera);
};


#endif // __SRC_LEVEL_TILELAYER_HPP_
