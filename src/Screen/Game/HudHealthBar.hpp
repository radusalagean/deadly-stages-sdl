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
    void dispose();

    Vector2D computeSize();

private:
    const int cellHeight = 35;
    const int cellWidth = 21;
    const float innerCellHeightUnitInterval = 25.0f / 35.0f;
    const float borderWidthUnitInterval = 1.0f / 35.0f;
    SDL_Color borderColor = {0, 0, 0, 255};
    SDL_Color backgroundColor = {255, 255, 255, 255};
    SDL_Color healthColor = {255, 0, 0, 255};

    const int numCells;
    int& filledCells;

    SDL_Rect allocatedRect;

    void drawCell(int x, int y, int w, int h, bool isFull);
};


#endif // __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__
