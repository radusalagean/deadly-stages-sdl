#ifndef __SRC_LEVEL_TILELAYER_HPP_
#define __SRC_LEVEL_TILELAYER_HPP_

#include <vector>

class Camera;
class Tile;

class TileLayer
{
public:
    int width = 0;
    int height = 0;
    std::vector<std::vector<Tile*>> tileMap{{}};

    void render(Camera& camera);
};


#endif // __SRC_LEVEL_TILELAYER_HPP_
