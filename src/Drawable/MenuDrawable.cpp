#include "MenuDrawable.hpp"

#include "../Game.hpp"
#include "../Drawable/MenuItemDrawable.hpp"
#include <algorithm>

void MenuDrawable::layout(int x, int y, int w, int h)
{
    int menuItemHeight = Game::height * 0.04;
    int menuItemSpacing = menuItemHeight * 0.2;
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
