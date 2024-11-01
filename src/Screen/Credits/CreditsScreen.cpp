#include "CreditsScreen.hpp"

#include "../../Game.hpp"
#include "../../Screen/MainMenu/MainMenuScreen.hpp"
#include "../../ScreenManager/ScreenManager.hpp"
#include "../../Drawable/TextDrawable.hpp"
#include "../../Core/Macros.hpp"
#include "../../Core/Constants.hpp"

CreditsScreen::CreditsScreen()
{
    for (auto& credit : credits)
    {
        std::vector<CreditDrawable> currentCreditDrawables;

        // Name
        CreditDrawable nameCreditDrawable {
            new TextDrawable(credit.creditOwner),
            0.07f,
            0.0f,
            0.01f
        };
        currentCreditDrawables.push_back(nameCreditDrawable);

        // Nickname
        if (!credit.nickname.empty())
        {
            CreditDrawable nicknameCreditDrawable {
                new TextDrawable("(" + credit.nickname + ")"),
                0.045f,
                0.01f,
                0.01f
            };
            currentCreditDrawables.push_back(nicknameCreditDrawable);
        }
        
        // Role
        CreditDrawable roleCreditDrawable {
            new TextDrawable(credit.role, Constants::COLOR_YELLOW_ACCENT),
            0.045f,
            0.01f,
            0.01f
        };
        currentCreditDrawables.push_back(roleCreditDrawable);
        
        // Link
        if (!credit.link.empty())
        {
            CreditDrawable linkCreditDrawable {
                new TextDrawable(credit.link, Constants::COLOR_LIGHT_GRAY_TEXT),
                0.035f,
                0.01f,
                0.01f
            };
            currentCreditDrawables.push_back(linkCreditDrawable);
        }

        // License
        if (!credit.license.empty())
        {
            CreditDrawable licenseCreditDrawable {
                new TextDrawable(credit.license, Constants::COLOR_DARK_GRAY_TEXT),
                0.03f,
                0.01f,
                0.0f
            };
            currentCreditDrawables.push_back(licenseCreditDrawable);
        }

        drawables.push_back(currentCreditDrawables);
    }

    // Footer
    CreditDrawable footerCreditDrawable {
        new TextDrawable(footerText, Constants::COLOR_YELLOW_ACCENT),
        0.08f,
        0.0f,
        0.0f
    };
    drawables.push_back(
        std::vector<CreditDrawable>{footerCreditDrawable}
    );
}

CreditsScreen::~CreditsScreen()
{
    for (auto& creditSection : drawables)
    {
        for (auto& drawable : creditSection)
        {
            delete drawable.textDrawable;
        }
    }
    drawables.clear();
}

void CreditsScreen::init()
{
    loadAssets();
}

void CreditsScreen::loadAssets()
{
    for (auto& creditSection : drawables)
    {
        for (auto& drawable : creditSection)
        {
            drawable.textDrawable->load();
        }
    }
}

void CreditsScreen::handleEvents()
{
    if (Game::control.isActionDown(CA_MENUS_BACK))
    {
        Game::screenManager.popScreen();
    }
}

void CreditsScreen::update()
{
    for (auto& creditSection : drawables)
    {
        for (auto& drawable : creditSection)
        {
            SDL_Rect* dstRect = &drawable.textDrawable->dstRect;
            drawable.textDrawable->setDstRect(dstRect->x, drawable.absoluteY + scrollOffsetPx, dstRect->w, dstRect->h);
            drawable.textDrawable->update();
        }
    }
    scrollOffsetPx -= Game::height * scrollSpeedUnitIntervalPerSecond * Game::minSecondsPerFrame;
    if (scrollOffsetPx + contentHeightPx <= 0)
    {
        Game::screenManager.popScreen();
    }
}

void CreditsScreen::render()
{
    for (auto& creditSection : drawables)
    {
        for (auto& drawable : creditSection)
        {
            drawable.textDrawable->draw();
        }
    }
}

void CreditsScreen::layoutPass()
{
    int currentY = 0;
    for (auto& creditSection : drawables)
    {
        for (auto& drawable : creditSection)
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
        if (&creditSection != &drawables.back())
        {
            currentY += USCALE(Game::height * 0.3);
        }
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