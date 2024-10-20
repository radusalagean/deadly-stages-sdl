#ifndef __SRC_CORE_VECTOR2D_HPP__
#define __SRC_CORE_VECTOR2D_HPP__

#include <math.h>
#include <utility>

#include "../Debug/Logger.hpp"

class Vector2D
{
public:
    Vector2D(float x, float y): x(x), y(y) {}
    Vector2D(): x(0), y(0) {}

    float x;
    float y;

    float length() { return sqrt(x * x + y * y); }

    void reset()
    {
        x = 0;
        y = 0;
    }

    Vector2D operator+(const Vector2D& v2) const
    {
        return Vector2D(x + v2.x, y + v2.y);
    }

    Vector2D& operator+=(const Vector2D& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2D operator*(float scalar) const
    {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator*(const Vector2D& v2) const
    {
        return Vector2D(x * v2.x, y * v2.y);
    }

    Vector2D& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2D operator-(const Vector2D& v2) const
    {
        return Vector2D(x - v2.x, y - v2.y);
    }

    Vector2D& operator-=(const Vector2D& v2)
    {
        x -= v2.x;
        y -= v2.y;
        return *this;
    }

    Vector2D operator/(float scalar)
    {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Vector2D getInverse() const { return Vector2D(1 / x, 1 / y); }

    void normalize()
    {
        float l = length();
        if (l > 0)
        {
            (*this) *= 1 / l;
        }
    }

    float magnitude() const { return sqrt(x * x + y * y); }
};

#endif // __SRC_CORE_VECTOR2D_HPP__