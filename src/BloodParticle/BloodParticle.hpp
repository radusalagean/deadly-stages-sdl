#ifndef __SRC_BLOODPARTICLE_BLOODPARTICLE_HPP__
#define __SRC_BLOODPARTICLE_BLOODPARTICLE_HPP__

#include "../Core/Vector2D.hpp"
#include <SDL.h>
#include "../Level/Camera.hpp"

class BloodParticle 
{
public:
    BloodParticle(Vector2D position, Vector2D velocity, float lifetime);
    void update();
    void render(const Camera& camera);
    float lifetime;
    static constexpr float MAX_LIFETIME_SECONDS = 0.7f;

private:
    Vector2D position;
    Vector2D velocity;
    static constexpr float GRAVITY = 200.0f;
};

#endif // __SRC_BLOODPARTICLE_BLOODPARTICLE_HPP__
