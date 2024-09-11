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
    std::vector<MenuItemDrawable*> menuItems;
    int selectedIndex = 0;
    Debouncer navigationDebouncer{500};

    void previousMenuItem();
    void nextMenuItem();
    void selectCurrentMenuItem();
};

#endif // __SRC_DRAWABLE_MENUDRAWABLE_HPP__
