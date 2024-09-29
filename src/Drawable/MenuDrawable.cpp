#include "MenuDrawable.hpp"

#include "../Game.hpp"
#include "../Drawable/MenuItemDrawable.hpp"
#include <algorithm>
#include "../Controls/Controls.hpp"
#include "../Core/Constants.hpp"
#include "../Core/Macros.hpp"

void MenuDrawable::layout(int x, int y, int w, int h)
{
    int menuItemHeight = USCALE(Game::height * 0.035);
    int menuItemSpacing = USCALE(menuItemHeight * 0.5);
    int currentY = y;
    int maxMenuItemWidth = 0;
    for (auto menuItem : menuItems)
    {
        int menuItemWidth = menuItemHeight * menuItem->getAspectRatio();
        if (menuItemWidth > maxMenuItemWidth)
        {
            maxMenuItemWidth = menuItemWidth;
        }
        int remainingHeight = std::max(0, h - (currentY - y));
        if (remainingHeight < menuItemHeight)
        {
            break;
        }
        menuItem->layout(x, currentY, menuItemWidth, menuItemHeight);
        currentY += menuItemHeight;
        if (menuItem != menuItems.back())
        {
            currentY += menuItemSpacing;
        }
    }
    if (w > maxMenuItemWidth) // Center the menu items
    {
        int newX = x + ((w - maxMenuItemWidth) / 2);
        layout(newX, y, maxMenuItemWidth, h);
    }
}

void MenuDrawable::load()
{
    for (auto menuItem : menuItems)
    {
        menuItem->load();
    }
}

void MenuDrawable::update()
{
    for (unsigned i = 0; i < menuItems.size(); i++)
    {
        menuItems[i]->selected = i == selectedIndex;
        menuItems[i]->update();
    }
}

void MenuDrawable::draw()
{
    for (auto menuItem : menuItems)
    {
        menuItem->draw();
    }
}

void MenuDrawable::dispose()
{
    for (auto menuItem : menuItems)
    {
        menuItem->dispose();
        delete menuItem;
    }
    menuItems.clear();
}

void MenuDrawable::setMenuItems(const std::vector<MenuItemDrawable*>& menuItems)
{
    this->menuItems = menuItems;
}

void MenuDrawable::previousMenuItem()
{
    if (menuItems.empty()) return;
    selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
}

void MenuDrawable::nextMenuItem()
{
    if (menuItems.empty()) return;
    selectedIndex = (selectedIndex + 1) % menuItems.size();
}

void MenuDrawable::selectCurrentMenuItem()
{
    if (menuItems.empty()) return;
    menuItems[selectedIndex]->pick();
}

void MenuDrawable::handleEvents()
{
    if (Game::controls.isActionDown(CA_UP))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_PREVIOUS))
        {
            previousMenuItem();
        }
    }
    else if (Game::controls.isActionUp(CA_UP))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_PREVIOUS);
    }

    if (Game::controls.isActionDown(CA_DOWN))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_NEXT))
        {
            nextMenuItem();
        }
    }
    else if (Game::controls.isActionUp(CA_DOWN))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_NEXT);
    }

    if (Game::controls.isActionDown(CA_SELECT))
    {
        selectCurrentMenuItem();
    }
}