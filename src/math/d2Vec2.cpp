#include "dura2d/d2Math.h"

d2Vec2
d2Vec2::Rotate(const d2Rot& rot) const
{
    d2Vec2 result;
    result.x = x * rot.c - y * rot.s;
    result.y = x * rot.s + y * rot.c;
    return result;
}

d2Vec2 &
d2Vec2::Normalize()
{
    float length = Lenght();
    if (length != 0.0) {
        x /= length;
        y /= length;
    }
    return *this;
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