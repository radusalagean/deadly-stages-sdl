#ifndef __SRC_CORE_CONSTANTS_HPP__
#define __SRC_CORE_CONSTANTS_HPP__

#include <SDL.h>
#include <string>

namespace Constants
{
    const std::string VERSION = "1.0.0";
    const int WINDOW_PADDING_PX = 16;
    const float UI_SCALE = 0.8f;
    
    const SDL_Color COLOR_RED = {255, 0, 0, 255};
    const SDL_Color COLOR_YELLOW_ACCENT = {240, 208, 26, 255};
    const SDL_Color COLOR_WHITE = {255, 255, 255, 255};
    const SDL_Color COLOR_LIGHT_GRAY_TEXT = {200, 200, 200, 255};
    const SDL_Color COLOR_DARK_GRAY_TEXT = {150, 150, 150, 255};
    const SDL_Color COLOR_BLACK = {0, 0, 0, 255};
}

#endif // __SRC_CORE_CONSTANTS_HPP__
