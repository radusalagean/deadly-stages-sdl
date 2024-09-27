#ifndef __SRC_LEVEL_CAMERA_HPP__
#define __SRC_LEVEL_CAMERA_HPP__

#include "../Core/Vector2D.hpp"

class Camera
{
public:

    Vector2D position;
    Vector2D* target = nullptr;
    Vector2D* mapBounds = nullptr;

    void update();
};

#endif // __SRC_LEVEL_CAMERA_HPP__
