#include "PermanentOverlayScreen.hpp"

#include "../../Game.hpp"
#include "../../Core/Macros.hpp"
#include "../../Core/PrimitiveShapeHelper.hpp"

PermanentOverlayScreen::PermanentOverlayScreen()
{
    drawables.push_back(&loadingTextDrawable);
    drawables.push_back(&savingTextDrawable);
}

PermanentOverlayScreen::~PermanentOverlayScreen()
{
    for (auto& drawable : drawables)
    {
        delete drawable;
    }
    drawables.clear();
}

void PermanentOverlayScreen::init()
{
    for (auto& drawable : drawables)
    {
        drawable->load();
    }
    framerateIndicator.init();
}

void PermanentOverlayScreen::update()
{
    framerateIndicator.update();
}

void PermanentOverlayScreen::render()
{
    
    if (Game::isLoading)
    {
        Game::primitiveShapeHelper.drawRect(loadingTextDrawable.dstRect, Constants::COLOR_BLACK);
        loadingTextDrawable.draw();
    }
    else if (Game::isSaving)
    {
        Game::primitiveShapeHelper.drawRect(savingTextDrawable.dstRect, Constants::COLOR_BLACK);
        savingTextDrawable.draw();
    }
    framerateIndicator.render();
}

void PermanentOverlayScreen::layoutPass()
{
    int textHeight = USCALE(Game::height * 0.045);
    { // Loading...
        int textWidth = textHeight * loadingTextDrawable.getAspectRatio();
        int x = Game::width - textWidth - Constants::WINDOW_PADDING_PX;
        int y = Game::height - textHeight - Constants::WINDOW_PADDING_PX;
        loadingTextDrawable.layout(x, y, textWidth, textHeight);
    }
    { // Saving...
        int textWidth = textHeight * savingTextDrawable.getAspectRatio();
        int x = Game::width - textWidth - Constants::WINDOW_PADDING_PX;
        int y = Game::height - textHeight - Constants::WINDOW_PADDING_PX;
        savingTextDrawable.layout(x, y, textWidth, textHeight);
    }
    layoutInvalidated = false;
}