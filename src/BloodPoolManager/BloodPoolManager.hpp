#ifndef __SRC_BLOODPOOLMANAGER_BLOODPOOLMANAGER_HPP__
#define __SRC_BLOODPOOLMANAGER_BLOODPOOLMANAGER_HPP__

#include <SDL.h>
#include <vector>

class BloodPoolManager
{
public:
    BloodPoolManager();
    ~BloodPoolManager();

    std::pair<SDL_Texture*, int> getRandomBloodPoolTexture();

private:
    static const int NUM_BLOOD_POOL_TEXTURES = 10;
    static const int MIN_BLOOD_POOL_SIZE = 32;
    static const int MAX_BLOOD_POOL_SIZE = 48;
    std::vector<std::pair<SDL_Texture*, int>> bloodPoolTextures; // pair of texture and size
};

#endif // __SRC_BLOODPOOLMANAGER_BLOODPOOLMANAGER_HPP__
