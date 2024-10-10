#ifndef __SRC_CORE_TEXTUREPOOL_HPP__
#define __SRC_CORE_TEXTUREPOOL_HPP__

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

class TexturePool
{
public:
    TexturePool();
    ~TexturePool();

    SDL_Texture* loadIfNeededAndGet(const std::string& path);

    std::map<std::string, SDL_Texture*> textures;
};

#endif // __SRC_CORE_TEXTUREPOOL_HPP__
