#ifndef __SRC_CORE_SDLUTILS_HPP__
#define __SRC_CORE_SDLUTILS_HPP__

#include <SDL.h>
#include <string>
#include "../Game.hpp"

namespace SDLUtils
{
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* createShadowTexture(SDL_Texture* originalTexture);

    extern SDL_Color tempColor;
    void pushTempRendererDrawColor();
    void popTempRendererDrawColor();
    bool areColorsEqual(const SDL_Color& color1, const SDL_Color& color2);
}

#endif // __SRC_CORE_SDLUTILS_HPP__
