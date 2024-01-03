#ifndef VEC2_H
#define VEC2_H

#include "dura2d/d2api.h"

struct D2_API d2Vec2
{
    float x;
    float y;

    d2Vec2();

    d2Vec2(float x, float y);

    ~d2Vec2() = default;

    void Add(const d2Vec2 &v);                 // v1.Add(v2)
    void Sub(const d2Vec2 &v);                 // v1.Sub(v2)
    void Scale(const float n);               // v1.Scale(n)
    d2Vec2 Rotate(const float angle) const;    // v1.Rotate(angle)

    float Magnitude() const;                 // v1.Magnitude()
    float MagnitudeSquared() const;          // v1.MagnitudeSquared()

    d2Vec2 &Normalize();                       // v1.Normalize()
    d2Vec2 UnitVector() const;                 // v1.UnitVector()
    d2Vec2 Normal() const;                     // n = v1.Normal()

    float Dot(const d2Vec2 &v) const;          // v1.Dot(v2)
    float Cross(const d2Vec2 &v) const;        // v1.Cross(v2)

    d2Vec2 &operator=(const d2Vec2 &v);        // v1 = v2
    bool operator==(const d2Vec2 &v) const;  // v1 == v2
    bool operator!=(const d2Vec2 &v) const;  // v1 != v2

    d2Vec2 operator+(const d2Vec2 &v) const;   // v1 + v2
    d2Vec2 operator-(const d2Vec2 &v) const;   // v1 - v2
    d2Vec2 operator*(const float n) const;   // v1 * n
    d2Vec2 operator/(const float n) const;   // v1 / n
    d2Vec2 operator-();                      // -v1

    d2Vec2 &operator+=(const d2Vec2 &v);       // v1 += v2
    d2Vec2 &operator-=(const d2Vec2 &v);       // v1 -= v2
    d2Vec2 &operator*=(const float n);       // v1 *= n
    d2Vec2 &operator/=(const float n);       // v1 /= n
};

#endif
