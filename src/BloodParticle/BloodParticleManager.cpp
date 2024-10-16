#include "BloodParticleManager.hpp"
#include <random>
#include "../Game.hpp"

void BloodParticleManager::createParticles(const Vector2D& position, int count) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> angleDist(0, 2 * M_PI);
    std::uniform_real_distribution<> speedDist(50, 150);
    std::uniform_real_distribution<> lifetimeDist(0.2f, 0.5f);

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

void BloodParticleManager::render(const Vector2D& cameraPosition) 
{
    for (auto& particle : particles) 
    {
        particle.render(cameraPosition);
    }
}
