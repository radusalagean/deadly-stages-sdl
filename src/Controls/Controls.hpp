#ifndef __SRC_CONTROLS_CONTROLS_HPP__
#define __SRC_CONTROLS_CONTROLS_HPP__

#include <SDL.h>

namespace Controls
{
    extern SDL_Event event;

    // Game controller
    extern SDL_GameController* gameController;

    void init();
    void handleEvent(SDL_Event& event);
}

#endif // __SRC_CONTROLS_CONTROLS_HPP__
