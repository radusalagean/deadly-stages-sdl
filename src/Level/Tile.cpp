#include "Tile.hpp"

#include "../Core/Macros.hpp"

Tile::Tile(int id, std::string path, int width, int height, bool collidable)
    : id(id), path(path), collidable(collidable), width(width), height(height)
{

}

Tile::~Tile()
{
    SDL_DestroyTexture(texture);
}

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
    dstRect.w = GSCALE(width);
    dstRect.h = GSCALE(height);
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}