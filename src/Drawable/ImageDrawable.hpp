#ifndef __SRC_DRAWABLE_IMAGEDRAWABLE_HPP__
#define __SRC_DRAWABLE_IMAGEDRAWABLE_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <string>

#include "Drawable.hpp"

class ImageDrawable : public Drawable
{
private:
    std::string fileName;
    std::string parentDirectory;
    SDL_Texture* texture = nullptr;

public:
    ImageDrawable(const std::string& fileName, 
        const std::string& parentDirectory = "res/image/");
    ImageDrawable(SDL_Texture* texture);
    ~ImageDrawable();
    
    int transparency = 255;
    bool externalTexture = false;

    SDL_Rect* srcRect = nullptr;
    void setSrcRect(int x, int y, int w, int h);
    
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
};

#endif // __SRC_DRAWABLE_IMAGEDRAWABLE_HPP__