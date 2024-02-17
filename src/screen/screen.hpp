#ifndef DEADLYSTAGES_SCREEN_HPP
#define DEADLYSTAGES_SCREEN_HPP

#include <SDL2/SDL.h>

class Screen
{
private:
protected:
    SDL_Renderer* renderer = NULL;
    Screen(SDL_Renderer* renderer);
public:
    virtual void init();
    virtual bool update();
    virtual void render();
    virtual void dispose();
};

#endif