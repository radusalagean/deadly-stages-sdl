#include "BloodPool.hpp"
#include "../Core/SDLUtils.hpp"
#include "../Game.hpp"
#include "../Core/PrimitiveShapeHelper.hpp"
#include <algorithm>
#include "../Core/SDLUtils.hpp"

BloodPool::BloodPool(const Vector2D& position) : GameEntity()
{
    gen = std::mt19937(rd());
    std::uniform_int_distribution<> radiusDis(MIN_BLOOD_POOL_SIZE / 2, MAX_BLOOD_POOL_SIZE / 2);
    radius = radiusDis(gen);
    setSize(radius * 2, radius * 2);
    setPosition(position - Vector2D(radius, radius));
    generateBloodTexture();
}

BloodPool::~BloodPool()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
}

void BloodPool::generateBloodTexture()
{
    int diameter = radius * 2;
    SDL_Color color = {255, 0, 0, 255};
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_ARGB8888);

    SDL_LockSurface(surface);

    Uint32* pixels = static_cast<Uint32*>(surface->pixels);
    Uint32 colorValue = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
    Uint32 transparentColor = SDL_MapRGBA(surface->format, 0, 0, 0, 0);

    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < diameter; ++y) 
    {
        for (int x = 0; x < diameter; ++x) 
        {
            int dx = x - radius;
            int dy = y - radius;
            double distanceFromCenter = std::sqrt(dx*dx + dy*dy);
            
            if (distanceFromCenter <= radius) 
            {
                double probability = 1.0 - (distanceFromCenter / radius);
                if (dis(gen) < probability) 
                {
                    pixels[y * diameter + x] = colorValue;
                }
                else 
                {
                    pixels[y * diameter + x] = transparentColor;
                }
            }
            else
            {
                pixels[y * diameter + x] = transparentColor;
            }
        }
    }

    SDL_UnlockSurface(surface);

    texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_FreeSurface(surface);
}
