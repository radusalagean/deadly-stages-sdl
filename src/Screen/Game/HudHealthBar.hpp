#ifndef __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__
#define __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__

#include "../../Drawable/Drawable.hpp"
#include <SDL.h>
#include "../../Core/Vector2D.hpp"
#include "../../Core/Constants.hpp"

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
    SDL_Color borderColor = Constants::COLOR_BLACK;
    SDL_Color backgroundColor = Constants::COLOR_WHITE;
    SDL_Color healthColor = Constants::COLOR_RED;

    const int numCells;
    int& filledCells;
    int cellWidth = 0;

    void drawCell(int x, int y, int w, int h, bool isFull);
};


#endif // __SRC_SCREEN_GAME_HUDHEALTHBAR_HPP__
