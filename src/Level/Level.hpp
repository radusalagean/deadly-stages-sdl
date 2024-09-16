#ifndef __SRC_LEVEL_LEVEL_HPP_
#define __SRC_LEVEL_LEVEL_HPP_

#include <map>
#include "Tile.hpp"
#include "TileLayer.hpp"

class Level 
{
public:
    Level();

    void load();
    void update();
    void render(Camera& camera);
    void dispose();

    std::map<int, Tile> tileset{};

    TileLayer tileLayer;

    int width = 0;
    int height = 0;
};

#endif // __SRC_LEVEL_LEVEL_HPP_
