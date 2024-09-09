#ifndef __SRC_DRAWABLE_DRAWABLE_HPP__
#define __SRC_DRAWABLE_DRAWABLE_HPP__

class Drawable
{
public:
    virtual ~Drawable() = default;
    virtual void layout(int x, int y, int w, int h) = 0;
    virtual void load() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void dispose() = 0;

    float getAspectRatio() const { return sourceAspectRatio; }

protected:
    SDL_Rect dstRect{0, 0, 0, 0};
    int sourceWidth = 0;
    int sourceHeight = 0;
    float sourceAspectRatio = 0.0f;
};

#endif // __SRC_DRAWABLE_DRAWABLE_HPP__
