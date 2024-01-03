#include "dura2d/d2CollisionDetection.h"
#include <limits>

bool
d2CollisionDetection::IsColliding(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contacts);
    }
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contacts);
    }
    if (aIsPolygon && bIsCircle) {
        return IsCollidingPolygonCircle(a, b, contacts);
    }
    if (aIsCircle && bIsPolygon) {
        return IsCollidingPolygonCircle(b, a, contacts);
    }
    return false;
}

bool
d2CollisionDetection::IsCollidingCircleCircle(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts)
{
    d2CircleShape *aCircleShape = (d2CircleShape *) a->shape;
    d2CircleShape *bCircleShape = (d2CircleShape *) b->shape;

    const d2Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }
    d2Contact contact;
    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.Normalize();

    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    contacts.push_back(contact);

    return true;
}

bool
d2CollisionDetection::IsCollidingPolygonPolygon(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts)
{
    auto *aPolygonShape = (d2PolygonShape *) a->shape;
    auto *bPolygonShape = (d2PolygonShape *) b->shape;
    int aIndexReferenceEdge, bIndexReferenceEdge;
    d2Vec2 aSupportPoint, bSupportPoint;
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aIndexReferenceEdge, aSupportPoint);
    if (abSeparation >= 0) {
        return false;
    }
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bIndexReferenceEdge, bSupportPoint);
    if (baSeparation >= 0) {
        return false;
    }

    d2PolygonShape *referenceShape;
    d2PolygonShape *incidentShape;
    int indexReferenceEdge;
    if (abSeparation > baSeparation) {
        referenceShape = aPolygonShape;
        incidentShape = bPolygonShape;
        indexReferenceEdge = aIndexReferenceEdge;
    } else {
        referenceShape = bPolygonShape;
        incidentShape = aPolygonShape;
        indexReferenceEdge = bIndexReferenceEdge;
    }

    // Find the reference edge based on the index that returned from the function
    d2Vec2 referenceEdge = referenceShape->EdgeAt(indexReferenceEdge);

    ///////////////////////////////////// 
    // Clipping 
    /////////////////////////////////////
    int incidentIndex = incidentShape->FindIncidentEdge(referenceEdge.Normal());
    int incidentNextIndex = (incidentIndex + 1) % incidentShape->worldVertices.size();
    d2Vec2 v0 = incidentShape->worldVertices[incidentIndex];
    d2Vec2 v1 = incidentShape->worldVertices[incidentNextIndex];

    std::vector<d2Vec2> contactPoints = {v0, v1};
    std::vector<d2Vec2> clippedPoints = contactPoints;
    for (int i = 0; i < referenceShape->worldVertices.size(); i++) {
        if (i == indexReferenceEdge)
            continue;
        d2Vec2 c0 = referenceShape->worldVertices[i];
        d2Vec2 c1 = referenceShape->worldVertices[(i + 1) % referenceShape->worldVertices.size()];
        int numClipped = referenceShape->ClipSegmentToLine(contactPoints, clippedPoints, c0, c1);
        if (numClipped < 2) {
            break;
        }

        contactPoints = clippedPoints; // make the next contact points the ones that were just clipped
    }

    auto vref = referenceShape->worldVertices[indexReferenceEdge];

    // Loop all clipped points, but only consider those where separation is negative (objects are penetrating each other)
    for (auto &vclip: clippedPoints) {
        float separation = (vclip - vref).Dot(referenceEdge.Normal());
        if (separation <= 0) {
            d2Contact contact;
            contact.a = a;
            contact.b = b;
            contact.normal = referenceEdge.Normal();
            contact.start = vclip;
            contact.end = vclip + contact.normal * -separation;
            if (baSeparation >= abSeparation) {
                std::swap(contact.start, contact.end); // the start-end points are always from "a" to "b"
                contact.normal *= -1.0;                // the collision normal is always from "a" to "b"
            }

            contacts.push_back(contact);
        }
    }
    return true;
}

bool
d2CollisionDetection::IsCollidingPolygonCircle(d2Body *polygon, d2Body *circle, std::vector<d2Contact> &contacts)
{
    const d2PolygonShape *polygonShape = (d2PolygonShape *) polygon->shape;
    const d2CircleShape *circleShape = (d2CircleShape *) circle->shape;
    const std::vector<d2Vec2> &polygonVertices = polygonShape->worldVertices;

    bool isOutside = false;
    d2Vec2 minCurrVertex;
    d2Vec2 minNextVertex;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();

    // Loop all the edges of the polygon/box finding the nearest edge to the circle center
    for (int i = 0; i < polygonVertices.size(); i++) {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        d2Vec2 edge = polygonShape->EdgeAt(currVertex);
        d2Vec2 normal = edge.Normal();

        // Compare the circle center with the rectangle vertex
        d2Vec2 vertexToCircleCenter = circle->position - polygonVertices[currVertex];
        float projection = vertexToCircleCenter.Dot(normal);

        // If we found a dot product projection that is in the positive/outside side of the normal
        if (projection > 0) {
            // Circle center is outside the polygon
            distanceCircleEdge = projection;
            minCurrVertex = polygonVertices[currVertex];
            minNextVertex = polygonVertices[nextVertex];
            isOutside = true;
            break;
        } else {
            // Circle center is inside the rectangle, find the min edge (the one with the least negative projection)
            if (projection > distanceCircleEdge) {
                distanceCircleEdge = projection;
                minCurrVertex = polygonVertices[currVertex];
                minNextVertex = polygonVertices[nextVertex];
            }
        }
    }

    d2Contact contact;

    if (isOutside) {
        ///////////////////////////////////////
        // Check if we are inside region A:
        ///////////////////////////////////////
        d2Vec2 v1 = circle->position - minCurrVertex; // vector from the nearest vertex to the circle center
        d2Vec2 v2 = minNextVertex - minCurrVertex; // the nearest edge (from curr vertex to next vertex)
        if (v1.Dot(v2) < 0) {
            // Distance from vertex to circle center is greater than radius... no collision
            if (v1.Magnitude() > circleShape->radius) {
                return false;
            } else {
                // Detected collision in region A:
                contact.a = polygon;
                contact.b = circle;
                contact.depth = circleShape->radius - v1.Magnitude();
                contact.normal = v1.Normalize();
                contact.start = circle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        } else {
            ///////////////////////////////////////
            // Check if we are inside region B:
            ///////////////////////////////////////
            v1 = circle->position - minNextVertex; // vector from the next nearest vertex to the circle center
            v2 = minCurrVertex - minNextVertex;   // the nearest edge
            if (v1.Dot(v2) < 0) {
                // Distance from vertex to circle center is greater than radius... no collision
                if (v1.Magnitude() > circleShape->radius) {
                    return false;
                } else {
                    // Detected collision in region B:
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - v1.Magnitude();
                    contact.normal = v1.Normalize();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            } else {
                ///////////////////////////////////////
                // We are inside region C:
                ///////////////////////////////////////
                if (distanceCircleEdge > circleShape->radius) {
                    // No collision... Distance between the closest distance and the circle center is greater than the radius.
                    return false;
                } else {
                    // Detected collision in region C:
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - distanceCircleEdge;
                    contact.normal = (minNextVertex - minCurrVertex).Normal();
                    contact.start = circle->position - (contact.normal * circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
        }
    } else {
        // The center of circle is inside the polygon... it is definitely colliding!
        contact.a = polygon;
        contact.b = circle;
        contact.depth = circleShape->radius - distanceCircleEdge;
        contact.normal = (minNextVertex - minCurrVertex).Normal();
        contact.start = circle->position - (contact.normal * circleShape->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }

    contacts.push_back(contact);

    return true;
}
