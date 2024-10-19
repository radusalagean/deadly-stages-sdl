#ifndef __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__
#define __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__

#include "BloodParticle.hpp"
#include <vector>
#include "../Core/Vector2D.hpp"
#include "../Level/Camera.hpp"

class BloodParticleManager 
{
public:
    void createParticles(const Vector2D& position, int count);
    void update();
    void render(const Camera& camera);

private:
    std::vector<BloodParticle> particles;
};

#endif // __SRC_BLOODPARTICLE_BLOODPARTICLEMANAGER_HPP__
