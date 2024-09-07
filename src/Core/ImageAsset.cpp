#include "ImageAsset.hpp"

ImageAsset::ImageAsset(const char* path, SDL_Rect dstRect)
{
    this->path = path;
    this->dstRect = dstRect;
}

void ImageAsset::load()
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(Game::renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void ImageAsset::render()
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}

void ImageAsset::dispose()
{
    SDL_DestroyTexture(texture);
}