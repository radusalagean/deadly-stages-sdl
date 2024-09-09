#ifndef __SRC_CORE_IMAGEASSET_HPP__
#define __SRC_CORE_IMAGEASSET_HPP__

#include <SDL.h>
#include <SDL_image.h>

#include "Drawable.hpp"

class ImageAsset : public Drawable
{
private:
    const char* path;
    SDL_Rect dstRect{0, 0, 0, 0};
    int sourceWidth = 0;
    int sourceHeight = 0;
    float sourceAspectRatio = 0.0f;

public:
    SDL_Texture* texture = nullptr;

    ImageAsset(const char* path);
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
    void dispose();

    float getAspectRatio() const { return sourceAspectRatio; }
};

#endif // __SRC_CORE_IMAGEASSET_HPP__