#ifndef D2DRAW_H
#define D2DRAW_H

#include "d2api.h"
#include "d2Math.h"
#include "d2Types.h"

/**
 * Color for debug drawing. Each value is in the range [0,1].
 */
struct D2_API d2Color
{
    d2Color() = default;

    d2Color(float rIn, float gIn, float bIn, float aIn = 1.0f)
    {
        r = rIn;
        g = gIn;
        b = bIn;
        a = aIn;
    }

    void Set(float rIn, float gIn, float bIn, float aIn = 1.0f)
    {
        r = rIn;
        g = gIn;
        b = bIn;
        a = aIn;
    }

    // * operator
    d2Color operator*(const float &f) const
    {
        return d2Color(r * f, g * f, b * f, a * f);
    }

    // *= operator
    d2Color &operator*=(const float &f)
    {
        r *= f;
        g *= f;
        b *= f;
        a *= f;
        return *this;
    }

    // / operator
    d2Color operator/(const float &f) const
    {
        return {r / f, g / f, b / f, a / f};
    }

    // /= operator
    d2Color &operator/=(const float &f)
    {
        r /= f;
        g /= f;
        b /= f;
        a /= f;
        return *this;
    }

    float r{}, g{}, b{}, a{1.0f};
};

class D2_API d2Draw
{
public:
    d2Draw() = default;

    virtual ~d2Draw(){}

    enum
    {
        e_shapeBit = 0x0001,        ///< draw shapes
        e_meshBit = 0x0002,         ///< draw meshes
        e_aabbBit = 0x0004,         ///< draw axis aligned bounding boxes
        e_aabbTreeBit = 0x0008,     ///< draw AABB tree
        e_transformBit = 0x0010,    ///< draw transforms
        e_jointBit = 0x0020,        ///< draw joints
    };

    void SetFlags(uint32 flags);

    uint32 GetFlags() const;

    void AppendFlags(uint32 flags);

    void ClearFlags(uint32 flags);

    virtual void DrawPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const d2Color &color) = 0;

    virtual void DrawSolidPolygon(const d2Vec2 *vertices, int32 vertexCount, const float &angle, const bool &mesh, const d2Color &color) = 0;

    virtual void DrawCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color) = 0;

    virtual void DrawSolidCircle(const d2Vec2 &center, float radius, const float &angle, const d2Color &color) = 0;

    virtual void DrawTransform(const d2Transform & transform) = 0;

    virtual void DrawSegment(const d2Vec2 &p1, const d2Vec2 &p2, const d2Color &color) = 0;

protected:
    uint32 m_flags{e_shapeBit};
};

inline void
d2Draw::SetFlags(uint32 flags)
{
    m_flags = flags;
}

inline uint32
d2Draw::GetFlags() const
{
    return m_flags;
}

inline void
d2Draw::AppendFlags(uint32 flags)
{
    m_flags |= flags;
}

inline void
d2Draw::ClearFlags(uint32 flags)
{
    m_flags &= ~flags;
}


#endif //D2DRAW_H
