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
        printf("Failed to load image at path: %s - %s\n", fullPath.c_str(), SDL_GetError());

        Game::handleCriticalError(
            "Texture Loading Error",
            "Failed to load image:\n" + fullPath + "\n\n" + SDL_GetError()
        );
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
        printf("Failed to create shadow texture: %s\n", SDL_GetError());
        return nullptr;
    }

    // Set the shadow texture as the render target
    if (SDL_SetRenderTarget(Game::renderer, shadowTexture) != 0) {
        printf("Failed to set shadow texture as render target: %s\n", SDL_GetError());
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Clear the shadow texture with transparent black
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
    SDL_RenderClear(Game::renderer);

    // Copy the original texture to the shadow texture
    if (SDL_RenderCopy(Game::renderer, originalTexture, nullptr, nullptr) != 0) {
        printf("Failed to copy original texture to shadow texture: %s\n", SDL_GetError());
        SDL_SetRenderTarget(Game::renderer, nullptr);
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Reset the render target
    SDL_SetRenderTarget(Game::renderer, nullptr);

    // Set the blend mode to allow alpha blending
    if (SDL_SetTextureBlendMode(shadowTexture, SDL_BLENDMODE_BLEND) != 0) {
        printf("Failed to set texture blend mode: %s\n", SDL_GetError());
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Set the color mod to black
    if (SDL_SetTextureColorMod(shadowTexture, 0, 0, 0) != 0) {
        printf("Failed to set texture color mod: %s\n", SDL_GetError());
        SDL_DestroyTexture(shadowTexture);
        return nullptr;
    }

    // Set the alpha modulation for the shadow effect
    if (SDL_SetTextureAlphaMod(shadowTexture, 64) != 0) {
        printf("Failed to set texture alpha mod: %s\n", SDL_GetError());
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

bool SDLUtils::areColorsEqual(const SDL_Color& color1, const SDL_Color& color2)
{
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}