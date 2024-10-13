#ifndef __SRC_CORE_SDLUTILS_HPP__
#define __SRC_CORE_SDLUTILS_HPP__

#include <SDL.h>
#include <string>
#include "../Debug/Logger.hpp"
#include "../Game.hpp"

namespace SDLUtils
{
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* createShadowTexture(SDL_Texture* originalTexture);

    extern SDL_Color tempColor;
    void pushTempRendererDrawColor();
    void popTempRendererDrawColor();
}

#endif // __SRC_CORE_SDLUTILS_HPP__
