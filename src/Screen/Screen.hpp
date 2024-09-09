#ifndef __SRC_SCREEN_SCREEN_HPP__
#define __SRC_SCREEN_SCREEN_HPP__

#include <SDL.h>
#include <vector>

class Drawable;

class Screen
{
private:
protected:
    Screen();

    std::vector<Drawable*> drawables{};
    bool layoutInvalidated = true;
    
public:
    virtual ~Screen() = default;
    virtual void init();
    virtual void handleEvents();
    virtual void layoutPass();
    virtual void update();
    virtual void render();
    virtual void dispose();

    void invalidateLayout() { layoutInvalidated = true; }
    bool isLayoutInvalidated() const { return layoutInvalidated; }
};

#endif // __SRC_SCREEN_SCREEN_HPP__