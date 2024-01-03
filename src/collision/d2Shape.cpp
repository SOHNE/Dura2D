#include "dura2d/d2Shape.h"
#include <iostream>
#include <limits>

d2CircleShape::d2CircleShape(float radius)
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

float
d2CircleShape::GetMomentOfInertia() const
{
    // For solid circles, the moment of inertia is 1/2 * r^2
    // But this still needs to be multiplied by the rigidbody's mass
    return 0.5 * (radius * radius);
}

d2PolygonShape::d2PolygonShape(const std::vector<d2Vec2> vertices)
{
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    // Initialize the vertices of the polygon shape and set width and height
    for (auto vertex: vertices) {
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);

        // Find min and max X and Y to calculate polygon width and height
        minX = std::min(minX, vertex.x);
        maxX = std::max(maxX, vertex.x);
        minY = std::min(minY, vertex.y);
        maxY = std::max(maxY, vertex.y);
    }
    width = maxX - minX;
    height = maxY - minY;
}

d2PolygonShape::~d2PolygonShape()
{

}

d2ShapeType
d2PolygonShape::GetType() const
{
    return POLYGON;
}

d2Shape *
d2PolygonShape::Clone() const
{
    return new d2PolygonShape(localVertices);
}

float
d2PolygonShape::PolygonArea() const
{
    float area = 0.0;
    for (int i = 0; i < localVertices.size(); i++) {
        int j = (i + 1) % localVertices.size();
        area += localVertices[i].Cross(localVertices[j]);
    }
    return area / 2.0;
}

d2Vec2
d2PolygonShape::PolygonCentroid() const
{
    d2Vec2 cg{0, 0};
    for (int i = 0; i < localVertices.size(); i++) {
        int j = (i + 1) % localVertices.size();
        cg += (localVertices[i] + localVertices[j]) * localVertices[i].Cross(localVertices[j]);
    }
    return cg / 6 / PolygonArea();
}

float
d2PolygonShape::GetMomentOfInertia() const
{
    float acc0 = 0;
    float acc1 = 0;
    for (int i = 0; i < localVertices.size(); i++) {
        auto a = localVertices[i];
        auto b = localVertices[(i + 1) % localVertices.size()];
        auto cross = abs(a.Cross(b));
        acc0 += cross * (a.Dot(a) + b.Dot(b) + a.Dot(b));
        acc1 += cross;
    }
    return acc0 / 6 / acc1;
}

d2Vec2
d2PolygonShape::EdgeAt(int index) const
{
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

float
d2PolygonShape::FindMinSeparation(const d2PolygonShape *other, int &indexReferenceEdge, d2Vec2 &supportPoint) const
{
    float separation = std::numeric_limits<float>::lowest();
    // Loop all the vertices of "this" polygon
    for (int i = 0; i < this->worldVertices.size(); i++) {
        d2Vec2 va = this->worldVertices[i];
        d2Vec2 normal = this->EdgeAt(i).Normal();
        // Loop all the vertices of the "other" polygon
        float minSep = std::numeric_limits<float>::max();
        d2Vec2 minVertex;
        for (int j = 0; j < other->worldVertices.size(); j++) {
            d2Vec2 vb = other->worldVertices[j];
            float proj = (vb - va).Dot(normal);
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
    float minProj = std::numeric_limits<float>::max();
    for (int i = 0; i < this->worldVertices.size(); ++i) {
        auto edgeNormal = this->EdgeAt(i).Normal();
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
    float dist0 = (contactsIn[0] - c0).Cross(normal);
    float dist1 = (contactsIn[1] - c0).Cross(normal);

    // If the points are behind the plane
    if (dist0 <= 0)
        contactsOut[numOut++] = contactsIn[0];
    if (dist1 <= 0)
        contactsOut[numOut++] = contactsIn[1];

    // If the points are on different sides of the plane (one distance is negative and the other is positive)
    if (dist0 * dist1 < 0) {
        float totalDist = dist0 - dist1;

        // Fint the intersection using linear interpolation: lerp(start,end) => start + t*(end-start)
        float t = dist0 / (totalDist);
        d2Vec2 contact = contactsIn[0] + (contactsIn[1] - contactsIn[0]) * t;
        contactsOut[numOut] = contact;
        numOut++;
    }
    return numOut;
}

void
d2PolygonShape::UpdateVertices(float angle, const d2Vec2 &position)
{
    // Loop all the vertices, transforming from local to world space
    for (int i = 0; i < localVertices.size(); i++) {
        // First rotate, then we translate
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

d2BoxShape::d2BoxShape(float width, float height)
{
    this->width = width;
    this->height = height;

    // Load the vertices of the box polygon
    localVertices.push_back(d2Vec2(-width / 2.0, -height / 2.0));
    localVertices.push_back(d2Vec2(+width / 2.0, -height / 2.0));
    localVertices.push_back(d2Vec2(+width / 2.0, +height / 2.0));
    localVertices.push_back(d2Vec2(-width / 2.0, +height / 2.0));

    worldVertices.push_back(d2Vec2(-width / 2.0, -height / 2.0));
    worldVertices.push_back(d2Vec2(+width / 2.0, -height / 2.0));
    worldVertices.push_back(d2Vec2(+width / 2.0, +height / 2.0));
    worldVertices.push_back(d2Vec2(-width / 2.0, +height / 2.0));
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

float
d2BoxShape::GetMomentOfInertia() const
{
    // For a rectangle, the moment of inertia is 1/12 * (w^2 + h^2)
    // But this still needs to be multiplied by the rigidbody's mass
    return (0.083333F) * (width * width + height * height);
}
