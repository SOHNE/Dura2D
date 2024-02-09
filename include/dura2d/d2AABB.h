#ifndef D2AABB_H
#define D2AABB_H

#include "d2api.h"
#include "d2Math.h"

struct D2_API d2AABB
{
    d2Vec2 lowerBound{};
    d2Vec2 upperBound{};

    d2AABB() = default;

    d2AABB(const d2Vec2& lower, const d2Vec2& upper) : lowerBound(lower), upperBound(upper) {}

    d2Vec2 GetCenter() const
    {
        return 0.5f * (lowerBound + upperBound);
    }

    d2Vec2 GetExtents() const
    {
        return 0.5f * (upperBound - lowerBound);
    }

    float GetPerimeter() const
    {
        float wx = upperBound.x - lowerBound.x;
        float wy = upperBound.y - lowerBound.y;
        return 2.0f * (wx + wy);
    }

    void Combine(const d2AABB& aabb)
    {
        lowerBound = d2Min(lowerBound, aabb.lowerBound);
        upperBound = d2Max(upperBound, aabb.upperBound);
    }

    void Combine(const d2AABB& aabb1, const d2AABB& aabb2)
    {
        lowerBound = d2Min(aabb1.lowerBound, aabb2.lowerBound);
        upperBound = d2Max(aabb1.upperBound, aabb2.upperBound);
    }

    bool Overlaps(const d2AABB& aabb) const
    {
        bool xOverlap = lowerBound.x <= aabb.upperBound.x && upperBound.x >= aabb.lowerBound.x;
        bool yOverlap = lowerBound.y <= aabb.upperBound.y && upperBound.y >= aabb.lowerBound.y;

        return xOverlap && yOverlap;
    }

    bool Contains(const d2AABB& aabb) const
    {
        bool xContains = lowerBound.x <= aabb.lowerBound.x && aabb.upperBound.x <= upperBound.x;
        bool yContains = lowerBound.y <= aabb.lowerBound.y && aabb.upperBound.y <= upperBound.y;

        return xContains && yContains;
    }

    bool Contains(const d2Vec2& point) const
    {
        bool xContains = lowerBound.x <= point.x && point.x <= upperBound.x;
        bool yContains = lowerBound.y <= point.y && point.y <= upperBound.y;

        return xContains && yContains;
    }
};

#endif //D2AABB_H
