#include "TexturePool.hpp"

#include "../Game.hpp"
#include "SDLUtils.hpp"

TexturePool::TexturePool()
{

}

TexturePool::~TexturePool()
{
    for (auto& pair : textures) 
    {
        SDL_DestroyTexture(pair.second);
    }
}

SDL_Texture* TexturePool::loadIfNeededAndGet(const std::string& path)
{
    if (textures.find(path) != textures.end())
        return textures[path]; // Texture already loaded

    SDL_Texture* texture = SDLUtils::loadTexture(path);
    textures[path] = texture;
    return texture;
}