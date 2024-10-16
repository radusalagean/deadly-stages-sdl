#include "BloodParticle.hpp"
#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../Core/SDLUtils.hpp"

BloodParticle::BloodParticle(Vector2D position, Vector2D velocity, float lifetime)
    : lifetime(lifetime), position(position), velocity(velocity) {}

void BloodParticle::update() 
{
    position += velocity * Game::latestLoopDeltaTimeSeconds;
    velocity.y += GRAVITY * Game::latestLoopDeltaTimeSeconds;
    lifetime -= Game::latestLoopDeltaTimeSeconds;
}

void BloodParticle::render(const Vector2D& cameraPosition) 
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect particleRect = {
        static_cast<int>(GSCALE(position.x) - cameraPosition.x),
        static_cast<int>(GSCALE(position.y) - cameraPosition.y),
        static_cast<int>(ceil(GSCALE(2))),
        static_cast<int>(ceil(GSCALE(2)))
    };
    SDL_RenderFillRect(renderer, &particleRect);
    SDLUtils::popTempRendererDrawColor();
}
