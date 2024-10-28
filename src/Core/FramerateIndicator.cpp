#include "FramerateIndicator.hpp"

#include "../Game.hpp"
#include "Options.hpp"
#include "Constants.hpp"
#include "Macros.hpp"

FramerateIndicator::FramerateIndicator()
{
    
}

FramerateIndicator::~FramerateIndicator()
{
    delete framerateTextDrawable;
}

void FramerateIndicator::init()
{
    framerateTextDrawable = new TextDrawable("", Constants::COLOR_YELLOW_ACCENT);
    framerateTextDrawable->load();
}

void FramerateIndicator::update()
{
    if (!Game::options.showFramerate)
        return;

    int deltaTimeMs = Game::latestLoopDeltaTimeMs;

    currentCycleFrameCount++;
    currentCycleTimeMs += deltaTimeMs;
    if (deltaTimeMs <= 0)
        return;

    if (SDL_GetTicks() - lastCycleProcessingTimeMs >= targetCycleTimeMs) {
        float currentCycleTimeSeconds = currentCycleTimeMs / 1000.0f;
        int average = ceil(currentCycleFrameCount / currentCycleTimeSeconds);
        lastCycleFramerate = average;
        currentCycleFrameCount = 0;
        currentCycleTimeMs = 0;
        lastCycleProcessingTimeMs = SDL_GetTicks();
    }

    framerateTextDrawable->setText(std::to_string(lastCycleFramerate));
    #ifdef PLATFORM_PSP
        const float height = USCALE(Game::height * 0.04);
    #else
        const float height = USCALE(Game::height * 0.035);
    #endif
    int width = height * framerateTextDrawable->getAspectRatio();
    int x = Game::width - width - Constants::WINDOW_PADDING_PX;
    int y = Constants::WINDOW_PADDING_PX;
    framerateTextDrawable->layout(x, y, width, height);
    framerateTextDrawable->update();
}

void FramerateIndicator::render()
{
    if (!Game::options.showFramerate)
        return;
    framerateTextDrawable->draw();
}