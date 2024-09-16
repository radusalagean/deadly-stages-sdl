#include "Tile.hpp"

void Tile::load()
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    texture = SDL_CreateTextureFromSurface(Game::renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
}

void Tile::draw(int x, int y)
{
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = width;
    dstRect.h = height;
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}