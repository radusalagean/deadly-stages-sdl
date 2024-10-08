#include "ImageDrawable.hpp"

#include "../Game.hpp"
#include "../Core/SDLUtils.hpp"
#include <string>

ImageDrawable::ImageDrawable(const std::string& fileName, const std::string& parentDirectory)
{
    this->fileName = fileName;
    this->parentDirectory = parentDirectory;
}

ImageDrawable::~ImageDrawable()
{
    SDL_DestroyTexture(texture);
}

void ImageDrawable::load()
{
    std::string path = parentDirectory + fileName;
    texture = SDLUtils::loadTexture(path);

    SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
    aspectRatio = (float)sourceWidth / (float)sourceHeight;
}

void ImageDrawable::layout(int x, int y, int w, int h)
{
    setDstRect(x, y, w, h);
}

void ImageDrawable::update()
{

}

void ImageDrawable::draw()
{
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}
