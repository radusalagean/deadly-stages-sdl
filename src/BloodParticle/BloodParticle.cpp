#include "BloodParticle.hpp"
#include "../Game.hpp"
#include "../Core/Macros.hpp"
#include "../Core/SDLUtils.hpp"
#include "../Core/Constants.hpp"

BloodParticle::BloodParticle(Vector2D position, Vector2D velocity, float lifetime)
    : lifetime(lifetime), position(position), velocity(velocity) {}

void BloodParticle::update() 
{
    position += velocity * Game::latestLoopDeltaTimeSeconds;
    velocity.y += GRAVITY * Game::latestLoopDeltaTimeSeconds;
    lifetime -= Game::latestLoopDeltaTimeSeconds;
}

void BloodParticle::render(const Camera& camera) 
{
    SDL_Renderer* renderer = Game::renderer;
    SDLUtils::pushTempRendererDrawColor();
    SDL_SetRenderDrawColor(renderer, Constants::COLOR_RED.r, Constants::COLOR_RED.g, 
        Constants::COLOR_RED.b, Constants::COLOR_RED.a);
    SDL_Rect particleRect = {
        static_cast<int>(camera.scale(position.x) - camera.position.x),
        static_cast<int>(camera.scale(position.y) - camera.position.y),
        static_cast<int>(ceil(camera.scale(2))),
        static_cast<int>(ceil(camera.scale(2)))
    };
    SDL_RenderFillRect(renderer, &particleRect);
    SDLUtils::popTempRendererDrawColor();
}
