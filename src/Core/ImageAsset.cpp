#include "ImageAsset.hpp"

#include "../Game.hpp"

ImageAsset::ImageAsset(const char* path)
{
    this->path = path;
}

void ImageAsset::layout(int x, int y, int w, int h)
{
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
}

void ImageAsset::load()
{
    SDL_Surface* loadedSurface = IMG_Load(path);
    texture = SDL_CreateTextureFromSurface(Game::renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
    sourceAspectRatio = (float)sourceWidth / (float)sourceHeight;
}

void ImageAsset::update()
{

}

void ImageAsset::draw()
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}

void ImageAsset::dispose()
{
    SDL_DestroyTexture(texture);
}