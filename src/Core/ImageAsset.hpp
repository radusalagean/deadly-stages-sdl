#ifndef __SRC_CORE_IMAGEASSET_HPP__
#define __SRC_CORE_IMAGEASSET_HPP__

#include <SDL.h>
#include <SDL_image.h>

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
    void dispose();
};

#endif // __SRC_CORE_IMAGEASSET_HPP__