#include "dura2d/d2Force.h"
#include <algorithm>

d2Vec2
d2Force::GenerateDragForce(const d2Body &body, float k)
{
    d2Vec2 dragForce = d2Vec2(0, 0);
    if (body.GetVelocity().MagnitudeSquared() > 0) {
        // Calculate the drag direction (inverse of velocity unit vector)
        d2Vec2 dragDirection = body.GetVelocity().UnitVector() * -1.0;

        // Calculate the drag magnitude, k * |v|^2
        float dragMagnitude = k * body.GetVelocity().MagnitudeSquared();

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }
    return dragForce;
}

d2Vec2
d2Force::GenerateFrictionForce(const d2Body &body, float k)
{
    // Calculate the friction direction (inverse of velocity unit vector)
    d2Vec2 frictionDirection = body.GetVelocity().UnitVector() * -1.0;

    // Calculate the friction magnitude (just k, for now)
    float frictionMagnitude = k;

    // Calculate the final resulting friction force vector
    d2Vec2 frictionForce = frictionDirection * frictionMagnitude;
    return frictionForce;
}

d2Vec2
d2Force::GenerateGravitationalForce(const d2Body &a, const d2Body &b, float G, float minDistance, float maxDistance)
{
    // Calculate the distance between the two objects
    d2Vec2 d = (b.GetPosition() - a.GetPosition());

    float distanceSquared = d.MagnitudeSquared();

    // Clamp the values of the distance (to allow for some insteresting visual effects)
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    // Calculate the direction of the attraction force
    d2Vec2 attractionDirection = d.UnitVector();

    // Calculate the strength of the attraction force
    float attractionMagnitude = G * (a.GetMass() * b.GetMass()) / distanceSquared;

    // Calculate the final resulting attraction force vector
    d2Vec2 attractionForce = attractionDirection * attractionMagnitude;
    return attractionForce;
}

d2Vec2
d2Force::GenerateSpringForce(const d2Body &body, d2Vec2 anchor, float restLength, float k)
{
    // Calculate the distance between the anchor and the object
    d2Vec2 d = body.GetPosition() - anchor;

    // Find the spring displacement considering the rest length
    float displacement = d.Magnitude() - restLength;

    // Calculate the direction of the spring force
    d2Vec2 springDirection = d.UnitVector();

    // Calculate the magnitude of the spring force
    float springMagnitude = -k * displacement;

    // Calculate the final resulting spring force vector
    d2Vec2 springForce = springDirection * springMagnitude;
    return springForce;
}
