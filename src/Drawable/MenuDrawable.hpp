#ifndef __SRC_DRAWABLE_MENUDRAWABLE_HPP__
#define __SRC_DRAWABLE_MENUDRAWABLE_HPP__

#include "Drawable.hpp"
#include <vector>
#include "../Core/Debouncer.hpp"

class MenuItemDrawable;

class MenuDrawable : public Drawable
{
public:
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();
    void dispose();

    void setMenuItems(const std::vector<MenuItemDrawable*>& menuItems);

    void handleEvents();

private:
    enum NavigationAction
    {
        NAVIGATION_ACTION_PREVIOUS = 0,
        NAVIGATION_ACTION_NEXT
    };

    std::vector<MenuItemDrawable*> menuItems;
    int selectedIndex = 0;
    Debouncer navigationDebouncer;

    void previousMenuItem();
    void nextMenuItem();
    void selectCurrentMenuItem();
};

#endif // __SRC_DRAWABLE_MENUDRAWABLE_HPP__
