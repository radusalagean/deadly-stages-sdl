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

void TexturePool::load(const std::string& path)
{
    if (textures.find(path) != textures.end())
        return; // Texture already loaded

    textures[path] = SDLUtils::loadTexture(path);
}

SDL_Texture* TexturePool::get(const std::string& path)
{
    return textures[path];
}