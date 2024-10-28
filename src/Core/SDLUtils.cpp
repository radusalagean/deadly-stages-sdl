#include "SDLUtils.hpp"

#include "../Game.hpp"
#include "Macros.hpp"
#include "Config.hpp"

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
    #ifndef SUPPORTS_SHADOWS
    return nullptr;
    #endif

    int width, height;
    Uint32 format;
    SDL_QueryTexture(originalTexture, &format, nullptr, &width, &height);

    // Create a target texture with a known format
    SDL_Texture* shadowTexture = SDL_CreateTexture(Game::renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!shadowTexture) 
    {
        logSDLe();
        return nullptr;
    }

    // Set the shadow texture as the render target
    if (SDL_SetRenderTarget(Game::renderer, shadowTexture) != 0) {
        logSDLe();
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Clear the shadow texture with transparent black
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    SDL_RenderClear(Game::renderer);

    // Copy the original texture to the shadow texture
    if (SDL_RenderCopy(Game::renderer, originalTexture, nullptr, nullptr) != 0) {
        logSDLe();
        SDL_SetRenderTarget(Game::renderer, nullptr);
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Reset the render target
    SDL_SetRenderTarget(Game::renderer, nullptr);

    // Set the blend mode to allow alpha blending
    if (SDL_SetTextureBlendMode(shadowTexture, SDL_BLENDMODE_BLEND) != 0) {
        logSDLe();
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Set the color mod to black
    if (SDL_SetTextureColorMod(shadowTexture, 0, 0, 0) != 0) {
        logSDLe();
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Set the alpha modulation for the shadow effect
    if (SDL_SetTextureAlphaMod(shadowTexture, 64) != 0) {
        logSDLe();
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

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