#include "TextDrawable.hpp"

#include "../Game.hpp"
#include "../Core/FontManager.hpp"

TextDrawable::TextDrawable(const std::string& text, const SDL_Color& color, const std::string& fontFileName) 
    : fontFileName(fontFileName), text(text), color(color)
{
    font = Game::fontManager.getFont(fontFileName, 12); // Start with a default size of 12 so we can get the aspect ratio
}

TextDrawable::~TextDrawable()
{
    SDL_DestroyTexture(texture);
}

void TextDrawable::load()
{
    refreshTexture();
}

void TextDrawable::layout(int x, int y, int w, int h)
{
    // We won't stretch the text, so we'll use the height as the font size and re-create the texture
    font = Game::fontManager.getFont(fontFileName, h);
    refreshTexture();
    setDstRect(x, y, w, h);
}

void TextDrawable::update()
{

}

void TextDrawable::draw()
{
    SDL_RenderCopy(Game::renderer, texture, nullptr, &dstRect);
}

void TextDrawable::setText(const std::string& text)
{
    this->text = text;
    refreshTexture();
    int newWidth = dstRect.h * aspectRatio;
    if (newWidth != dstRect.w)
    {
        setDstRect(dstRect.x, dstRect.y, newWidth, dstRect.h);
    }
}

void TextDrawable::refreshTexture()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_QueryTexture(texture, NULL, NULL, &sourceWidth, &sourceHeight);
    aspectRatio = (float)sourceWidth / (float)sourceHeight;
}