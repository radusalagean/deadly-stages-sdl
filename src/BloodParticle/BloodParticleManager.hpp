#ifndef __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__
#define __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__

#include "BloodParticle.hpp"
#include <vector>
#include <random>
#include "../Core/Vector2D.hpp"
#include "../Level/Camera.hpp"

class BloodParticleManager 
{
public:
    BloodParticleManager();
    ~BloodParticleManager();

    void createParticles(const Vector2D& position, int count);
    void update();
    void render(const Camera& camera);

private:
    std::vector<BloodParticle> particles;

    std::mt19937 gen;
    std::uniform_real_distribution<> angleDist;
    std::uniform_real_distribution<> speedDist;
    std::uniform_real_distribution<> lifetimeDist;
};

#endif // __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__
