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
        SDL_DestroyTexture(pair.second.first);
        if (pair.second.second != nullptr)
            SDL_DestroyTexture(pair.second.second);
    }
}

std::pair<SDL_Texture*, SDL_Texture*> TexturePool::loadIfNeededAndGet(const std::string& path, bool attachShadow)
{
    if (textures.find(path) != textures.end()) // Texture already loaded
    {
        SDL_Texture* texture = textures[path].first;
        SDL_Texture* shadowTexture = textures[path].second;
        if (attachShadow && shadowTexture == nullptr)
        {
            shadowTexture = SDLUtils::createShadowTexture(texture);
            textures[path].second = shadowTexture;
        }
        return textures[path];
    }

    SDL_Texture* texture = SDLUtils::loadTexture(path);
    SDL_Texture* shadowTexture = nullptr;
    if (attachShadow)
    {
        shadowTexture = SDLUtils::createShadowTexture(texture);
    }
    textures[path] = std::make_pair(texture, shadowTexture);
    return textures[path];
}