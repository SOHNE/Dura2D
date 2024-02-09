#include "dura2d/d2Shape.h"
#include <iostream>
#include <limits>

d2CircleShape::d2CircleShape(real radius)
{
    this->radius = radius;
}

d2CircleShape::~d2CircleShape()
{
}

d2Shape *
d2CircleShape::Clone() const
{
    return new d2CircleShape(radius);
}

d2ShapeType
d2CircleShape::GetType() const
{
    return CIRCLE;
}

real
d2CircleShape::GetMomentOfInertia() const
{
    // For solid circles, the moment of inertia is 1/2 * r^2
    // But this still needs to be multiplied by the rigidbody's mass
    return 0.5F * (radius * radius);
}

d2PolygonShape::d2PolygonShape(const d2Vec2* vertices, int vertexCount)
{
    real minX = std::numeric_limits<real>::max();
    real minY = std::numeric_limits<real>::max();
    real maxX = std::numeric_limits<real>::lowest();
    real maxY = std::numeric_limits<real>::lowest();

    localVertices = new d2Vec2[vertexCount];
    worldVertices = new d2Vec2[vertexCount];

    // Initialize the vertices of the polygon shape and set width and height
    for (int i = 0; i < vertexCount; ++i)
    {
        localVertices[i] = vertices[i];
        worldVertices[i] = vertices[i];

        // Find min and max X and Y to calculate polygon width and height
        minX = d2Min(minX, vertices[i].x);
        maxX = d2Max(maxX, vertices[i].x);
        minY = d2Min(minY, vertices[i].y);
        maxY = d2Max(maxY, vertices[i].y);
    }

    width = maxX - minX;
    height = maxY - minY;

    m_vertexCount = vertexCount;
}

d2PolygonShape::~d2PolygonShape()
{
    delete[] localVertices;
    delete[] worldVertices;
}

d2ShapeType
d2PolygonShape::GetType() const
{
    return POLYGON;
}

d2Shape *
d2PolygonShape::Clone() const
{
    return new d2PolygonShape(localVertices, m_vertexCount);
}

real
d2PolygonShape::PolygonArea() const
{
    real area = 0.0;
    for (int i = 0; i < m_vertexCount; i++) {
        int j = (i + 1) % m_vertexCount;
        area += localVertices[i].Cross(localVertices[j]);
    }
    return area / 2.0F;
}

d2Vec2
d2PolygonShape::PolygonCentroid() const
{
    d2Vec2 cg{0, 0};
    for (int i = 0; i < m_vertexCount; i++) {
        int j = (i + 1) % m_vertexCount;
        cg += (localVertices[i] + localVertices[j]) * localVertices[i].Cross(localVertices[j]);
    }
    return cg / 6 / PolygonArea();
}

real
d2PolygonShape::GetMomentOfInertia() const
{
    real acc0 = 0;
    real acc1 = 0;
    for (int i = 0; i < m_vertexCount; i++) {
        auto a = localVertices[i];
        auto b = localVertices[(i + 1) % m_vertexCount];
        auto cross = d2Abs(a.Cross(b));
        acc0 += cross * (a.Dot(a) + b.Dot(b) + a.Dot(b));
        acc1 += cross;
    }
    return acc0 / 6 / acc1;
}

d2Vec2
d2PolygonShape::EdgeAt(int index) const
{
    int currVertex = index;
    int nextVertex = (index + 1) % m_vertexCount;
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

real
d2PolygonShape::FindMinSeparation(const d2PolygonShape *other, int &indexReferenceEdge, d2Vec2 &supportPoint) const
{
    real separation = std::numeric_limits<real>::lowest();
    // Loop all the vertices of "this" polygon
    for (int i = 0; i < this->m_vertexCount; i++) {
        d2Vec2 va = this->worldVertices[i];
        d2Vec2 normal = this->EdgeAt(i).Normal();
        // Loop all the vertices of the "other" polygon
        real minSep = std::numeric_limits<real>::max();
        d2Vec2 minVertex;
        for (int j = 0; j < other->m_vertexCount; j++) {
            d2Vec2 vb = other->worldVertices[j];
            real proj = (vb - va).Dot(normal);
            if (proj < minSep) {
                minSep = proj;
                minVertex = vb;
            }
        }
        if (minSep > separation) {
            separation = minSep;
            indexReferenceEdge = i;
            supportPoint = minVertex;
        }
    }
    return separation;
}

int
d2PolygonShape::FindIncidentEdge(const d2Vec2 &normal) const
{
    int indexIncidentEdge;
    real minProj = std::numeric_limits<real>::max();
    for (int i = 0; i < m_vertexCount; ++i) {
        auto edgeNormal = EdgeAt(i).Normal();
        auto proj = edgeNormal.Dot(normal);
        if (proj < minProj) {
            minProj = proj;
            indexIncidentEdge = i;
        }
    }
    return indexIncidentEdge;
}

int
d2PolygonShape::ClipSegmentToLine(const std::vector<d2Vec2> &contactsIn,
                                  std::vector<d2Vec2> &contactsOut,
                                  const d2Vec2 &c0,
                                  const d2Vec2 &c1) const
{
    // Start with no output points
    int numOut = 0;

    // Calculate the distance of end points to the line
    d2Vec2 normal = (c1 - c0).Normalize();
    real dist0 = (contactsIn[0] - c0).Cross(normal);
    real dist1 = (contactsIn[1] - c0).Cross(normal);

    // If the points are behind the plane
    if (dist0 <= 0)
        contactsOut[numOut++] = contactsIn[0];
    if (dist1 <= 0)
        contactsOut[numOut++] = contactsIn[1];

    // If the points are on different sides of the plane (one distance is negative and the other is positive)
    if (dist0 * dist1 < 0) {
        real totalDist = dist0 - dist1;

        // Fint the intersection using linear interpolation: lerp(start,end) => start + t*(end-start)
        real t = dist0 / (totalDist);
        d2Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numOut] = contact;
        numOut++;
    }
    return numOut;
}

void
d2PolygonShape::UpdateVertices(const d2Transform &transform)
{
    // Loop all the vertices, transforming from local to world space
    for (int i = 0; i < m_vertexCount; i++) {
        d2Vec2 rotated = {
                transform.q.c * localVertices[i].x - transform.q.s * localVertices[i].y,
                transform.q.s * localVertices[i].x + transform.q.c * localVertices[i].y
        };
        worldVertices[i] = rotated + transform.p;
    }
}

d2BoxShape::d2BoxShape(real width, real height)
{
    this->width = width;
    this->height = height;

    // Load the vertices of the box polygon
    m_vertexCount = 4;
    d2Vec2 vertices[4] = {
        d2Vec2(-width / 2.0F, -height / 2.0F),
        d2Vec2(+width / 2.0F, -height / 2.0F),
        d2Vec2(+width / 2.0F, +height / 2.0F),
        d2Vec2(-width / 2.0F, +height / 2.0F)
    };

    // Allocate memory for the local and world vertices
    localVertices = new d2Vec2[m_vertexCount];
    worldVertices = new d2Vec2[m_vertexCount];

    // Copy the calculated vertices to both arrays
    std::copy(std::begin(vertices), std::end(vertices), localVertices);
    std::copy(std::begin(vertices), std::end(vertices), worldVertices);
}

d2BoxShape::~d2BoxShape()
{

}

d2ShapeType
d2BoxShape::GetType() const
{
    return BOX;
}

d2Shape *
d2BoxShape::Clone() const
{
    return new d2BoxShape(width, height);
}

real
d2BoxShape::GetMomentOfInertia() const
{
    // For a rectangle, the moment of inertia is 1/12 * (w^2 + h^2)
    // But this still needs to be multiplied by the rigidbody's mass
    return (0.083333F) * (width * width + height * height);
}
