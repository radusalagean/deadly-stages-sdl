#ifndef __SRC_LEVEL_TILE_HPP_
#define __SRC_LEVEL_TILE_HPP_

#include <string>
#include <SDL.h>
#include "../Game.hpp"
#include <SDL_image.h>

class Tile
{
public:
    int id = 0;
    std::string path = "";
    int width = 0;
    int height = 0;
    bool collidable = false;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

    void load();
    void draw(int x, int y);
};

#endif // __SRC_LEVEL_TILE_HPP_
