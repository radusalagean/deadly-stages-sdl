#ifndef __SRC_CORE_CONSTANTS_HPP__
#define __SRC_CORE_CONSTANTS_HPP__

namespace Constants
{
    const std::string VERSION = "1.0.0-dev";
    const int WINDOW_PADDING_PX = 16;
    const float UI_SCALE = 0.8f;
    const float GAMEPLAY_SCALE = 1.0f;
    const int FPS_LIMIT = 60;
    const int MIN_MILLIS_PER_FRAME = 1000 / FPS_LIMIT;
    const SDL_Color COLOR_YELLOW_ACCENT = {240, 208, 26, 255};
}

#endif // __SRC_CORE_CONSTANTS_HPP__
