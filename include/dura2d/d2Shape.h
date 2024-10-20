#ifndef SHAPE_H
#define SHAPE_H

#include "d2api.h"
#include "d2Math.h"
#include <vector>

#include "memory/d2BlockAllocator.h"

enum D2_API d2ShapeType
{
    CIRCLE,
    POLYGON,
    BOX
};

struct D2_API d2Shape
{
    virtual ~d2Shape() = default;

    virtual d2ShapeType GetType() const = 0;

    // clone with block allocator as parameter
    virtual d2Shape *Clone() const = 0;

    virtual void UpdateVertices(const d2Transform &transform) = 0;

    virtual real GetMomentOfInertia() const = 0;
};

struct D2_API d2CircleShape : public d2Shape
{
    real radius;

    d2CircleShape(const real radius);

    virtual ~d2CircleShape();

    d2ShapeType GetType() const override;

    d2Shape *Clone() const override;

    void UpdateVertices(const d2Transform &transform) override { (void)transform; };

    real GetMomentOfInertia() const override;
};

struct D2_API d2PolygonShape : public d2Shape
{
    real width;
    real height;

    int m_vertexCount;

    d2Vec2* localVertices;
    d2Vec2* worldVertices;

    d2PolygonShape() = default;

    d2PolygonShape(const d2Vec2* vertices, int vertexCount);

    virtual ~d2PolygonShape();

    d2ShapeType GetType() const override;

    d2Shape *Clone() const override;

    d2Vec2 EdgeAt(int index) const;

    real FindMinSeparation(const d2PolygonShape *other, int &indexReferenceEdge, d2Vec2 &supportPoint) const;

    int FindIncidentEdge(const d2Vec2 &normal) const;

    int ClipSegmentToLine(const std::vector<d2Vec2> &contactsIn,
                          std::vector<d2Vec2> &contactsOut,
                          const d2Vec2 &c0,
                          const d2Vec2 &c1) const;

    real PolygonArea() const;

    d2Vec2 PolygonCentroid() const;

    real GetMomentOfInertia() const override;

    void UpdateVertices(const d2Transform &transform) override;

    friend class d2World;
};

struct D2_API d2BoxShape : public d2PolygonShape
{
    d2BoxShape(real width, real height);

    virtual ~d2BoxShape();

    d2ShapeType GetType() const override;

    d2Shape *Clone() const override;

    real GetMomentOfInertia() const override;
};

#endif
