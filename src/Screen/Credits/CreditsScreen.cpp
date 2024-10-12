#include "CreditsScreen.hpp"

#include "../../Game.hpp"
#include "../../Screen/MainMenu/MainMenuScreen.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Core/Macros.hpp"

CreditsScreen::CreditsScreen()
{
    for (auto& credit : credits)
    {
        // Name
        CreditDrawable nameCreditDrawable {
            new TextDrawable(credit.name),
            0.08f,
            0.0f,
            0.01f
        };
        drawables.push_back(nameCreditDrawable);

        // Nickname
        if (!credit.nickname.empty())
        {
            CreditDrawable nicknameCreditDrawable {
                new TextDrawable("(" + credit.nickname + ")"),
                0.08f,
                0.0f,
                0.0f
            };
            drawables.push_back(nicknameCreditDrawable);
        }
        
        // Role
        CreditDrawable roleCreditDrawable {
            new TextDrawable(credit.role, {200, 200, 200, 255}),
            0.06f,
            0.04f,
            0.3f
        };
        drawables.push_back(roleCreditDrawable);
    }

    // Footer
    CreditDrawable footerCreditDrawable {
        new TextDrawable(footerText),
        0.1f,
        0.0f,
        0.0f
    };
    drawables.push_back(footerCreditDrawable);
}

CreditsScreen::~CreditsScreen()
{
    for (auto drawable : drawables)
    {
        delete drawable.textDrawable;
    }
    drawables.clear();
}

void CreditsScreen::init()
{
    loadAssets();
}

void CreditsScreen::loadAssets()
{
    for (auto drawable : drawables)
    {
        drawable.textDrawable->load();
    }
}

void CreditsScreen::handleEvents()
{
    if (Game::control.isActionDown(CA_ESCAPE))
    {
        Game::screenManager.popScreen();
    }
}

void CreditsScreen::update()
{
    for (auto drawable : drawables)
    {
        SDL_Rect* dstRect = &drawable.textDrawable->dstRect;
        drawable.textDrawable->setDstRect(dstRect->x, drawable.absoluteY + scrollOffsetPx, dstRect->w, dstRect->h);
        drawable.textDrawable->update();
    }
    scrollOffsetPx -= Game::height * scrollSpeedUnitIntervalPerSecond * Game::latestLoopDeltaTimeSeconds;
    if (scrollOffsetPx + contentHeightPx <= 0)
    {
        Game::screenManager.popScreen();
    }
}

void CreditsScreen::render()
{
    for (auto drawable : drawables)
    {
        drawable.textDrawable->draw();
    }
}

void CreditsScreen::layoutPass()
{
    int currentY = 0;
    for (auto& drawable : drawables)
    {
        int height = USCALE(Game::height * drawable.heightUnitInterval);
        int width = height * drawable.textDrawable->getAspectRatio();
        int x = (Game::width - width) / 2;
        int paddingTop = USCALE(Game::height * drawable.paddingTopUnitInterval);
        int paddingBottom = USCALE(Game::height * drawable.paddingBottomUnitInterval);
        int y = paddingTop + currentY;
        drawable.absoluteY = y;
        drawable.textDrawable->layout(x, y, width, height);
        currentY += paddingTop + height + paddingBottom;
    }
    contentHeightPx = currentY;
    if (scrollOffsetPx == FLT_MAX)
    {
        scrollOffsetPx = Game::height;
    }
    else
    {
        float heightDiffRatio = lastKnownWindowHeightPx == 0 ? 1.0f : Game::height / (float)lastKnownWindowHeightPx;
        scrollOffsetPx = scrollOffsetPx * heightDiffRatio;
    }
    lastKnownWindowHeightPx = Game::height;
    layoutInvalidated = false;
}