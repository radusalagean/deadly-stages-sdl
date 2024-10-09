#ifndef __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__
#define __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__

#include "../../Drawable/Drawable.hpp"
#include <SDL.h>
#include "../../Core/Vector2D.hpp"

class HudHealthBar : public Drawable
{
public:
    HudHealthBar(const int& numCells, int& filledCells);
    ~HudHealthBar();
    void layout(int x, int y, int w, int h);
    void load();
    void update();
    void draw();

    int getActualWidth();

private:
    const float innerCellHeightUnitInterval = 25.0f / 35.0f;
    const float borderWidthUnitInterval = 1.0f / 35.0f;
    SDL_Color borderColor = {0, 0, 0, 255};
    SDL_Color backgroundColor = {255, 255, 255, 255};
    SDL_Color healthColor = {255, 0, 0, 255};

    const int numCells;
    int& filledCells;
    int cellWidth = 0;

    void drawCell(int x, int y, int w, int h, bool isFull);
};


#endif // __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__
