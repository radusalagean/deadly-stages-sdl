#include "TexturePool.hpp"

#include "../Game.hpp"

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

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    textures[path] = SDL_CreateTextureFromSurface(Game::renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

SDL_Texture* TexturePool::get(const std::string& path)
{
    return textures[path];
}