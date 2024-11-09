#include "BloodParticleManager.hpp"
#include "../Game.hpp"
#include <cmath>

BloodParticleManager::BloodParticleManager() : 
    gen(std::random_device{}()),
    angleDist(0, 2 * M_PI),
    speedDist(50, 150),
    lifetimeDist(0.2f, 0.5f)
{}

BloodParticleManager::~BloodParticleManager() {}

void BloodParticleManager::createParticles(const Vector2D& position, int count) 
{
    for (int i = 0; i < count; ++i) 
    {
        float angle = angleDist(gen);
        float speed = speedDist(gen);
        Vector2D velocity(cos(angle) * speed, sin(angle) * speed);
        particles.emplace_back(position, velocity, lifetimeDist(gen));
    }
}

void BloodParticleManager::update() 
{
    for (auto it = particles.begin(); it != particles.end();) 
    {
        it->update();
        if (it->lifetime <= 0) 
        {
            it = particles.erase(it);
        } 
        else 
        {
            ++it;
        }
    }
}

void BloodParticleManager::render(const Camera& camera) 
{
    for (auto& particle : particles) 
    {
        particle.render(camera);
    }
}
