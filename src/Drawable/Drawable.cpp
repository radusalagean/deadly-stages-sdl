#include "Drawable.hpp"
#include <cassert>

void Drawable::setDstRect(int x, int y, int w, int h)
{
    dstRect.x = x;
    dstRect.y = y;
    dstRect.w = w;
    dstRect.h = h;
}