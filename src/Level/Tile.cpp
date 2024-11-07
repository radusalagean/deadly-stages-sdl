#include "Tile.hpp"

#include "../Core/Macros.hpp"
#include "../Core/SDLUtils.hpp"

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
    texture = SDLUtils::loadTexture(path);
}

void Tile::draw(Camera& camera, int x, int y, int scaledWidth, int scaledHeight)
{
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = scaledWidth;
    dstRect.h = scaledHeight;
    SDL_RenderCopy(Game::renderer, texture, nullptr, &dstRect);
}