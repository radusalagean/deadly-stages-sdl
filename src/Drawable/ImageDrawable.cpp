#include "ImageDrawable.hpp"

#include "../Game.hpp"
#include "../Core/SDLUtils.hpp"
#include <string>

ImageDrawable::ImageDrawable(const std::string& fileName, const std::string& parentDirectory) : 
    fileName(fileName),
    parentDirectory(parentDirectory),
    externalTexture(false) {}

ImageDrawable::ImageDrawable(SDL_Texture* texture) : 
    texture(texture), 
    externalTexture(true) {}

ImageDrawable::~ImageDrawable()
{
    if (!externalTexture)
        SDL_DestroyTexture(texture);
}

void ImageDrawable::load()
{
    if (!externalTexture)
    {
        std::string path = parentDirectory + fileName;
        texture = SDLUtils::loadTexture(path);
    }

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
    Uint8 initialAlpha;
    SDL_GetTextureAlphaMod(texture, &initialAlpha);
    SDL_SetTextureAlphaMod(texture, transparency);
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
    SDL_SetTextureAlphaMod(texture, initialAlpha);
}