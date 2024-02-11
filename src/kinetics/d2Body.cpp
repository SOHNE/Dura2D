#include "dura2d/d2Body.h"

#include <iostream>

#include "dura2d/d2AABB.h"
#include "dura2d/d2World.h"

d2Body::d2Body(const d2Shape &shape, real x, real y, real mass, d2World *world) : world(world)
{
    this->m_transform = d2Transform((d2Vec2(x, y)), d2Rot(0.F));

    this->velocity = d2Vec2(0, 0);
    this->acceleration = d2Vec2(0, 0);

    this->angularVelocity = 0.F;
    this->angularAcceleration = 0.F;

    this->sumForces = d2Vec2(0, 0);
    this->sumTorque = 0.F;

    this->restitution = 0.6F;
    this->friction = 0.7F;

    this->mass = mass;
    this->invMass = (mass != 0.F) ? 1.F / mass : 0.F;
    {
        const real epsilon = 0.005f;
        d2Abs(invMass - 0.0) < epsilon
            ? m_type = d2_staticBody
            : m_type = d2_dynamicBody;
    }

    this->I = shape.GetMomentOfInertia() * mass;
    this->invI = (I != 0.F) ? 1.F / I : 0.F;

    this->shape = shape.Clone();
    this->shape->UpdateVertices(m_transform);

    this->m_flags = d2Body::e_awakeFlag;
    this->m_sleepTime = 0.0F;

    // Create the d2AABB for this body
    aabb = new d2AABB();
    ComputeAABB();
}

d2Body::~d2Body()
{
    delete shape;
    delete aabb;
}

void
d2Body::ComputeAABB()
{
    switch (shape->GetType())
    {
        case POLYGON:
        case BOX:
        {
            auto* box = dynamic_cast<d2PolygonShape*>(shape);
            d2Vec2 minVertex = box->worldVertices[0];
            d2Vec2 maxVertex = box->worldVertices[0];

            for (int i = 1; i < 4; ++i) {
                minVertex = d2Min(minVertex, box->worldVertices[i]);
                maxVertex = d2Max(maxVertex, box->worldVertices[i]);
            }

            aabb->lowerBound = minVertex;
            aabb->upperBound = maxVertex;
            break;
        }
        case CIRCLE:
        {
            auto* circle = dynamic_cast<d2CircleShape*>(shape);
            d2Vec2 lowerBound = m_transform.p - d2Vec2(circle->radius, circle->radius);
            d2Vec2 upperBound = m_transform.p + d2Vec2(circle->radius, circle->radius);
            aabb->lowerBound = lowerBound;
            aabb->upperBound = upperBound;
            break;
        }
        default:
            std::cout << "d2Shape type not supported" << std::endl;
            break;
    }

    if (aabb->Collider == nullptr)
        aabb->Collider = this;
}

void
d2Body::AddForce(const d2Vec2 &force)
{
    sumForces += force;
}

void
d2Body::AddTorque(real torque)
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
    d2Vec2 rotated = d2Rotate(m_transform.q, point);
    return rotated + m_transform.p;
}

d2Vec2
d2Body::WorldSpaceToLocalSpace(const d2Vec2 &point) const
{
    const d2Vec2 translated = point - m_transform.p;
    return { m_transform.q.GetXAxis().Dot(translated), m_transform.q.GetYAxis().Dot(translated) };
}

void
d2Body::ApplyImpulseLinear(const d2Vec2 &j)
{
    if (m_type == d2_staticBody) return;
    velocity += j * invMass;
}

void
d2Body::ApplyImpulseAngular(const real j)
{
    if (m_type == d2_staticBody) return;
    angularVelocity += j * invI;
}

void
d2Body::ApplyImpulseAtPoint(const d2Vec2 &j, const d2Vec2 &r)
{
    if (m_type == d2_staticBody) return;
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void
d2Body::IntegrateForces(const real dt)
{
    if (m_type == d2_staticBody) return;

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
d2Body::IntegrateVelocities(const real dt)
{
    if (m_type == d2_staticBody) return;

    // Integrate the velocity to find the new position
    m_transform.p += velocity * dt;

    // Integrate the angular velocity to find the new rotation angle
    m_transform.q += angularVelocity * dt;

    // Update the vertices to adjust them to the new position/rotation
    shape->UpdateVertices(m_transform);
}
