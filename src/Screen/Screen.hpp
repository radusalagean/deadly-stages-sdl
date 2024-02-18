#ifndef DEADLYSTAGES_SCREEN_HPP
#define DEADLYSTAGES_SCREEN_HPP

#include <SDL2/SDL.h>

class Screen
{
private:
protected:
    Screen();
public:
    virtual void init();
    virtual bool update();
    virtual void render();
    virtual void dispose();
};

#endif