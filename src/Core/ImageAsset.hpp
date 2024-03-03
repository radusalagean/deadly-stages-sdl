#ifndef DEADLYSTAGES_IMAGE_ASSET_HPP
#define DEADLYSTAGES_IMAGE_ASSET_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class ImageAsset;

#include "../Game.hpp"

class ImageAsset
{
private:
    const char* path;
    SDL_Rect dstRect;

public:
    SDL_Texture* texture;
    
    ImageAsset(const char* path, SDL_Rect dstRect);
    void load();
    void render();
};

#endif