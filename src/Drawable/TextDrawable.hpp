#ifndef __SRC_DRAWABLE_TEXTDRAWABLE_HPP__
#define __SRC_DRAWABLE_TEXTDRAWABLE_HPP__

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Drawable.hpp"

class TextDrawable : public Drawable
{
private:
    std::string fontFileName;
    std::string text;
    SDL_Color color;
    TTF_Font* font = nullptr;
    SDL_Texture* texture = nullptr;

    void refreshTexture();

public:
    TextDrawable(const std::string& text, const SDL_Color& color = {255, 255, 255, 255}, 
        const std::string& fontFileName = "PressStart2P.ttf");
    ~TextDrawable();

    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
    void setText(const std::string& text);
    inline std::string getText() const { return text; }
};

#endif // __SRC_DRAWABLE_TEXTDRAWABLE_HPP__
