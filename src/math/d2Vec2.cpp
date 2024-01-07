#include "dura2d/d2Vec2.h"
#include <cmath>

d2Vec2::d2Vec2() : x(0.0), y(0.0)
{

}

d2Vec2::d2Vec2(float x, float y) : x(x), y(y)
{

}

void
d2Vec2::Add(const d2Vec2 &v)
{
    x += v.x;
    y += v.y;
}

void
d2Vec2::Sub(const d2Vec2 &v)
{
    x -= v.x;
    y -= v.y;
}

void
d2Vec2::Scale(const float n)
{
    x *= n;
    y *= n;
}

d2Vec2
d2Vec2::Rotate(const float angle) const
{
    d2Vec2 result;
    result.x = x * cos(angle) - y * sin(angle);
    result.y = x * sin(angle) + y * cos(angle);
    return result;
}

float
d2Vec2::Magnitude() const
{
    return sqrtf(x * x + y * y);
}

float
d2Vec2::MagnitudeSquared() const
{
    return (x * x + y * y);
}

d2Vec2 &
d2Vec2::Normalize()
{
    float length = Magnitude();
    if (length != 0.0) {
        x /= length;
        y /= length;
    }
    return *this;
}

d2Vec2
d2Vec2::UnitVector() const
{
    d2Vec2 result = d2Vec2(0, 0);
    float length = Magnitude();
    if (length != 0.0) {
        result.x = x / length;
        result.y = y / length;
    }
    return result;
}

d2Vec2
d2Vec2::Normal() const
{
    return d2Vec2(y, -x).Normalize();
}

float
d2Vec2::Dot(const d2Vec2 &v) const
{
    return (x * v.x) + (y * v.y);
}

float
d2Vec2::Cross(const d2Vec2 &v) const
{
    return (x * v.y) - (y * v.x);
}

d2Vec2 &
d2Vec2::operator=(const d2Vec2 &v)
{
    x = v.x;
    y = v.y;
    return *this;
}

bool
d2Vec2::operator==(const d2Vec2 &v) const
{
    return x == v.x && y == v.y;
}

bool
d2Vec2::operator!=(const d2Vec2 &v) const
{
    return !(*this == v);
}

d2Vec2
d2Vec2::operator+(const d2Vec2 &v) const
{
    d2Vec2 result;
    result.x = x + v.x;
    result.y = y + v.y;
    return result;
}

d2Vec2
d2Vec2::operator-(const d2Vec2 &v) const
{
    d2Vec2 result;
    result.x = x - v.x;
    result.y = y - v.y;
    return result;
}

d2Vec2
d2Vec2::operator*(const float n) const
{
    d2Vec2 result;
    result.x = x * n;
    result.y = y * n;
    return result;
}

d2Vec2
d2Vec2::operator*(const d2Vec2 &v) const
{
    d2Vec2 result;
    result.x = x * v.x;
    result.y = y * v.y;
    return result;
}

d2Vec2
d2Vec2::operator/(const float n) const
{
    d2Vec2 result;
    result.x = x / n;
    result.y = y / n;
    return result;
}

d2Vec2 &
d2Vec2::operator+=(const d2Vec2 &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

d2Vec2 &
d2Vec2::operator-=(const d2Vec2 &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

d2Vec2 &
d2Vec2::operator*=(const float n)
{
    x *= n;
    y *= n;
    return *this;
}

d2Vec2 &
d2Vec2::operator*=(const d2Vec2 &v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

d2Vec2 &
d2Vec2::operator/=(const float n)
{
    x /= n;
    y /= n;
    return *this;
}

d2Vec2
d2Vec2::operator-()
{
    d2Vec2 result;
    result.x = -x;
    result.y = -y;
    return result;
}
