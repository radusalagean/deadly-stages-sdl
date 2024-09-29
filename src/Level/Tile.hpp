#ifndef __SRC_LEVEL_TILE_HPP_
#define __SRC_LEVEL_TILE_HPP_

#include <string>
#include <SDL.h>
#include "../Game.hpp"
#include <SDL_image.h>
#include "../Core/Constants.hpp"

class Tile
{
private:
    int id;
    std::string path;
    
    bool collidable;
    SDL_Texture* texture = nullptr;
    SDL_Rect dstRect;

public:
    Tile(int id, std::string path, int width, int height, bool collidable);
    ~Tile();

    int width;
    int height;

    void load();
    void draw(int x, int y);
};

#endif // __SRC_LEVEL_TILE_HPP_
