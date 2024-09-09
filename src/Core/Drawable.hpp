#ifndef __SRC_CORE_DRAWABLE_HPP__
#define __SRC_CORE_DRAWABLE_HPP__

class Drawable
{
public:
    virtual ~Drawable() = default;
    virtual void layout(int x, int y, int w, int h) = 0;
    virtual void load() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void dispose() = 0;
};

#endif // __SRC_CORE_DRAWABLE_HPP__
