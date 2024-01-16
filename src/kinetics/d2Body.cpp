#include "dura2d/d2Body.h"
#include <cmath>
#include <iostream>

#include "dura2d/d2AABB.h"

d2Body::d2Body(const d2Shape &shape, float x, float y, float mass, d2World *world) : world(world)
{
    this->shape = shape.Clone();
    this->position = d2Vec2(x, y);
    this->velocity = d2Vec2(0, 0);
    this->acceleration = d2Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = d2Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 0.6;
    this->friction = 0.7;
    this->mass = mass;
    this->invMass = (mass != 0.0) ? 1.F / mass : 0.F;
    this->I = shape.GetMomentOfInertia() * mass;
    this->invI = (I != 0.0) ? 1.F / I : 0.F;
    this->shape->UpdateVertices(rotation, position);

    // Create the d2AABB for this body
    ComputeAABB();
}

d2Body::~d2Body()
{
    delete shape;
    delete aabb;
}

bool
d2Body::IsStatic() const
{
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
}

void
d2Body::ComputeAABB()
{
    if (aabb != nullptr) delete aabb;

    switch (shape->GetType())
    {
        case POLYGON:
        case BOX:
        {
            auto* box = dynamic_cast<d2BoxShape*>(this->shape);
            auto minX = std::min(box->worldVertices[0].x, std::min(box->worldVertices[1].x, std::min(box->worldVertices[2].x, box->worldVertices[3].x)));
            auto minY = std::min(box->worldVertices[0].y, std::min(box->worldVertices[1].y, std::min(box->worldVertices[2].y, box->worldVertices[3].y)));
            auto maxX = std::max(box->worldVertices[0].x, std::max(box->worldVertices[1].x, std::max(box->worldVertices[2].x, box->worldVertices[3].x)));
            auto maxY = std::max(box->worldVertices[0].y, std::max(box->worldVertices[1].y, std::max(box->worldVertices[2].y, box->worldVertices[3].y)));
            aabb = new d2AABB(minX, minY, maxX, maxY);
            break;
        }
        case CIRCLE:
        {
            auto* circle = dynamic_cast<d2CircleShape*>(this->shape);
            aabb = new d2AABB(position.x - circle->radius, position.y - circle->radius, position.x + circle->radius, position.y + circle->radius);
            break;
        }
        default:
            std::cout << "d2Shape type not supported" << std::endl;
            break;

    }
}

void
d2Body::AddForce(const d2Vec2 &force)
{
    sumForces += force;
}

void
d2Body::AddTorque(float torque)
{
    sumTorque += torque;
}

void
d2Body::ClearForces()
{
    sumForces = d2Vec2(0.0, 0.0);
}

void
d2Body::ClearTorque()
{
    sumTorque = 0.0;
}

d2Vec2
d2Body::LocalSpaceToWorldSpace(const d2Vec2 &point) const
{
    d2Vec2 rotated = point.Rotate(rotation);
    return rotated + position;
}

d2Vec2
d2Body::WorldSpaceToLocalSpace(const d2Vec2 &point) const
{
    float translatedX = point.x - position.x;
    float translatedY = point.y - position.y;
    float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
    float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;
    return {rotatedX, rotatedY};
}

void
d2Body::ApplyImpulseLinear(const d2Vec2 &j)
{
    if (IsStatic())
        return;
    velocity += j * invMass;
}

void
d2Body::ApplyImpulseAngular(const float j)
{
    if (IsStatic())
        return;
    angularVelocity += j * invI;
}

void
d2Body::ApplyImpulseAtPoint(const d2Vec2 &j, const d2Vec2 &r)
{
    if (IsStatic())
        return;
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void
d2Body::IntegrateForces(const float dt)
{
    if (IsStatic()) return;

    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorque * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;

    // Clear all the forces and torque acting on the object before the next physics step
    ClearForces();
    ClearTorque();
}

void
d2Body::IntegrateVelocities(const float dt)
{
    if (IsStatic())
        return;

    // Integrate the velocity to find the new position
    position += velocity * dt;

    // Integrate the angular velocity to find the new rotation angle
    rotation += angularVelocity * dt;

    // Update the vertices to adjust them to the new position/rotation
    shape->UpdateVertices(rotation, position);
}
