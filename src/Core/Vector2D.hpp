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

    float getX() const { return x; }
    float getY() const { return y; }

    float& getX() { return x; }
    float& getY() { return y; }

    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }

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

    friend Vector2D& operator-=(Vector2D& v1, const Vector2D& v2)
    {
        v1.x -= v2.x;
        v1.y -= v2.y;
        return v1;
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

private:
    float x;
    float y;
};

#endif // __SRC_CORE_VECTOR2D_HPP__