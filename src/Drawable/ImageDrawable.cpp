#include "ImageDrawable.hpp"

#include "../Game.hpp"
#include <string>

ImageDrawable::ImageDrawable(const std::string& fileName)
{
    this->fileName = fileName;
}

void ImageDrawable::load()
{
    std::string path = "res/image/" + fileName;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(Game::renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
    sourceAspectRatio = (float)sourceWidth / (float)sourceHeight;
}

void ImageDrawable::layout(int x, int y, int w, int h)
{
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
}

void ImageDrawable::update()
{

}

void ImageDrawable::draw()
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}

void ImageDrawable::dispose()
{
    SDL_DestroyTexture(texture);
}