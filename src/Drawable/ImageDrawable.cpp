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

    if (srcRect != nullptr)
        delete srcRect;
}

void ImageDrawable::load()
{
    if (!externalTexture)
    {
        std::string path = parentDirectory + fileName;
        texture = SDLUtils::loadTexture(path);
    }

    SDL_QueryTexture(texture, nullptr, nullptr, &sourceWidth, &sourceHeight);
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
    SDL_RenderCopy(Game::renderer, texture, srcRect, &dstRect);
    SDL_SetTextureAlphaMod(texture, initialAlpha);
}

void ImageDrawable::setSrcRect(int x, int y, int w, int h)
{
    if (srcRect != nullptr)
        delete srcRect;

    srcRect = new SDL_Rect();
    srcRect->x = x;
    srcRect->y = y;
    srcRect->w = w;
    srcRect->h = h;
}