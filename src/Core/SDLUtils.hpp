#ifndef __SRC_CORE_SDLUTILS_HPP__
#define __SRC_CORE_SDLUTILS_HPP__

#include <SDL.h>
#
#include <string>
#include "../Debug/Logger.hpp"
#include "../Game.hpp"

namespace SDLUtils
{
    SDL_Texture* loadTexture(const std::string& path);
}

#endif // __SRC_CORE_SDLUTILS_HPP__
