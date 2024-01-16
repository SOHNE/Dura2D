#ifndef DURA2D_D2AABB_H
#define DURA2D_D2AABB_H

#include "dura2d/d2Vec2.h"

class d2AABB
{
public:
    float minX{};
    float minY{};
    float maxX{};
    float maxY{};

    d2AABB(float minX, float minY, float maxX, float maxY)
            : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

    d2AABB() : minX(0), minY(0), maxX(0), maxY(0) {}

    bool Overlaps(const d2AABB &other) const
    {
        return (minX <= other.maxX && maxX >= other.minX) &&
               (minY <= other.maxY && maxY >= other.minY);
    }

    bool Contains(const d2Vec2 &point) const
    {
        return (point.x >= minX && point.x <= maxX) &&
               (point.y >= minY && point.y <= maxY);
    }

    inline float GetWidth() const { return maxX - minX; }

    inline float GetHeight() const { return maxY - minY; }
};

#endif //DURA2D_D2AABB_H
