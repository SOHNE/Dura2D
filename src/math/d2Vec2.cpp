#include "dura2d/d2Math.h"


d2Vec2 &
d2Vec2::Normalize()
{
    real length = Lenght();
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
d2Vec2::operator*(const real n) const
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
d2Vec2::operator/(const real n) const
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
d2Vec2::operator*=(const real n)
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
d2Vec2::operator/=(const real n)
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