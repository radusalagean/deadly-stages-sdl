#include "SDLUtils.hpp"

#include "../Game.hpp"
#include "Macros.hpp"

SDL_Texture* SDLUtils::loadTexture(const std::string& path)
{
    std::string fullPath = RPATH(path);
    SDL_Surface* surface = IMG_Load(fullPath.c_str());
    if (surface == nullptr)
    {
        logd("Failed to load image at path: %s", fullPath.c_str());
        logSDLe();
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

/**
 * Uses the alpha channel of the original texture to create a shadow texture.
 */
SDL_Texture* SDLUtils::createShadowTexture(SDL_Texture* originalTexture)
{
    int width, height;
    Uint32 format;
    SDL_QueryTexture(originalTexture, &format, NULL, &width, &height);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!surface)
    {
        logSDLe();
        return nullptr;
    }

    SDL_Texture* tempTexture = SDL_CreateTexture(Game::renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!tempTexture) 
    {
        logSDLe();
        SDL_FreeSurface(surface);
        return nullptr;
    }

    SDL_SetRenderTarget(Game::renderer, tempTexture);
    SDL_RenderCopy(Game::renderer, originalTexture, NULL, NULL);
    SDL_RenderReadPixels(Game::renderer, NULL, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(Game::renderer, NULL);

    SDL_Surface* shadowSurface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
    if (!shadowSurface) 
    {
        logSDLe();
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(tempTexture);
        return nullptr;
    }

    SDL_LockSurface(surface);
    SDL_LockSurface(shadowSurface);

    Uint32* srcPixels = (Uint32*)surface->pixels;
    Uint32* dstPixels = (Uint32*)shadowSurface->pixels;

    for (int i = 0; i < width * height; ++i) {
        Uint8 r, g, b, a;
        Uint32 pixel = srcPixels[i];
        SDL_GetRGBA(pixel, surface->format, &r, &g, &b, &a);
        dstPixels[i] = SDL_MapRGBA(shadowSurface->format, 0, 0, 0, a);
    }

    SDL_UnlockSurface(shadowSurface);
    SDL_UnlockSurface(surface);

    SDL_Texture* shadowTexture = SDL_CreateTextureFromSurface(Game::renderer, shadowSurface);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(shadowSurface);
    SDL_DestroyTexture(tempTexture);

    SDL_SetTextureAlphaMod(shadowTexture, 50);

    return shadowTexture;
}

SDL_Color SDLUtils::tempColor;

void SDLUtils::pushTempRendererDrawColor()
{
    SDL_GetRenderDrawColor(Game::renderer, &tempColor.r, &tempColor.g, &tempColor.b, &tempColor.a);
}

void SDLUtils::popTempRendererDrawColor()
{
    SDL_SetRenderDrawColor(Game::renderer, tempColor.r, tempColor.g, tempColor.b, tempColor.a);
}