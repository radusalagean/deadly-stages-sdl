#ifndef __SRC_SCREEN_SCREEN_HPP__
#define __SRC_SCREEN_SCREEN_HPP__

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

#endif // __SRC_SCREEN_SCREEN_HPP__