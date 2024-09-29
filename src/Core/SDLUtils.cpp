#include "SDLUtils.hpp"

SDL_Texture* SDLUtils::loadTexture(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        logd("Failed to load image at path: %s", path.c_str());
        logSDLe();
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}