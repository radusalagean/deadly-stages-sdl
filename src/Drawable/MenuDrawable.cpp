#include "MenuDrawable.hpp"

#include "../Game.hpp"
#include "../Drawable/MenuItemDrawable.hpp"
#include <algorithm>
#include "../Control/Control.hpp"
#include "../Core/Constants.hpp"
#include "../Core/Macros.hpp"

MenuDrawable::~MenuDrawable()
{
    for (auto menuItem : menuItems)
    {
        delete menuItem;
    }
    menuItems.clear();
}

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
    int menuHeight = currentY - y;
    if (w > maxMenuItemWidth || h > menuHeight)
    {
        int finalX = w > maxMenuItemWidth ? x + ((w - maxMenuItemWidth) / 2) : x;
        int finalY = h > menuHeight ? y + ((h - menuHeight) / 2) : y;
        layout(finalX, finalY, maxMenuItemWidth, menuHeight);
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
    if (Game::control.isActionDown(CA_MENUS_UP))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_PREVIOUS))
        {
            previousMenuItem();
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_UP))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_PREVIOUS);
    }

    if (Game::control.isActionDown(CA_MENUS_DOWN))
    {
        if (navigationDebouncer.canPerformAction(NAVIGATION_ACTION_NEXT))
        {
            nextMenuItem();
        }
    }
    else if (Game::control.isActionUp(CA_MENUS_DOWN))
    {
        navigationDebouncer.resetAction(NAVIGATION_ACTION_NEXT);
    }

    if (Game::control.isActionDown(CA_MENUS_SELECT))
    {
        selectCurrentMenuItem();
    }
}