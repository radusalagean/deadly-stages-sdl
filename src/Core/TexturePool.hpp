#ifndef __SRC_CORE_TEXTUREPOOL_HPP__
#define __SRC_CORE_TEXTUREPOOL_HPP__

#include <map>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <utility>

class TexturePool
{
public:
    TexturePool();
    ~TexturePool();

    // First: texture, Second: shadow texture (optional)
    std::pair<SDL_Texture*, SDL_Texture*> loadIfNeededAndGet(const std::string& path, bool attachShadow);

    std::map<std::string, std::pair<SDL_Texture*, SDL_Texture*>> textures;
};

#endif // __SRC_CORE_TEXTUREPOOL_HPP__
