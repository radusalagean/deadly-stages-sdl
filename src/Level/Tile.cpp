#include "Tile.hpp"

Tile::Tile(int id, std::string path, int width, int height, bool collidable)
    : id(id), path(path), width(width), height(height), collidable(collidable)
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
    dstRect.w = getScaledWidth();
    dstRect.h = getScaledHeight();
    SDL_RenderCopy(Game::renderer, texture, NULL, &dstRect);
}