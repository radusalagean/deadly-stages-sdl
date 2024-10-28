#include "BloodPoolManager.hpp"
#include "../Game.hpp"
#include <random>

BloodPoolManager::BloodPoolManager()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> radiusDis(MIN_BLOOD_POOL_SIZE / 2, MAX_BLOOD_POOL_SIZE / 2);
    for (int i = 0; i < NUM_BLOOD_POOL_TEXTURES; ++i)
    {
        int radius = radiusDis(gen);
        int diameter = radius * 2;

        SDL_Color color = {255, 0, 0, 255};
        SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, diameter, diameter, 32, SDL_PIXELFORMAT_ARGB8888);

        SDL_LockSurface(surface);

        Uint32* pixels = static_cast<Uint32*>(surface->pixels);
        Uint32 colorValue = SDL_MapRGBA(surface->format, color.r, color.g, color.b, color.a);
        Uint32 transparentColor = SDL_MapRGBA(surface->format, 0, 0, 0, 0);

        const int radiusSquared = radius * radius;
        
        std::vector<float> randomValues(diameter * diameter);
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for (int i = 0; i < diameter * diameter; ++i) 
        {
            randomValues[i] = dis(gen);
        }

        int randomIndex = 0;
        for (int y = 0; y < diameter; ++y) 
        {
            const int dy = y - radius;
            const int dySquared = dy * dy;
            
            for (int x = 0; x < diameter; ++x) 
            {
                const int dx = x - radius;
                const int distanceSquared = dx * dx + dySquared;
                
                if (distanceSquared <= radiusSquared) 
                {
                    float probability = 1.0f - (std::sqrt(static_cast<float>(distanceSquared)) / radius);
                    pixels[y * diameter + x] = (randomValues[randomIndex++] < probability) ? colorValue : transparentColor;
                }
                else
                {
                    pixels[y * diameter + x] = transparentColor;
                    randomIndex++;
                }
            }
        }

        SDL_UnlockSurface(surface);

        SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_FreeSurface(surface);
        
        bloodPoolTextures.push_back(texture);
    }
}

BloodPoolManager::~BloodPoolManager()
{
    for (auto& texture : bloodPoolTextures)
    {
        SDL_DestroyTexture(texture);
    }
    bloodPoolTextures.clear();
}

SDL_Texture* BloodPoolManager::getRandomBloodPoolTexture()
{
    int index = rand() % bloodPoolTextures.size();
    return bloodPoolTextures[index];
}