#ifndef BODY_H
#define BODY_H

#include "d2api.h"

#include "d2Vec2.h"
#include "d2Shape.h"

class AABB;

struct D2_API d2Body
{
    // Linear motion
    d2Vec2 position;
    d2Vec2 velocity;
    d2Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    d2Vec2 sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass;
    float I;
    float invI;

    // Coefficient of restitution (elasticity)
    float restitution;

    // Coefficient of friction
    float friction;

    // AABB
    AABB *aabb = nullptr;

    // Pointer to the shape/geometry of this rigid body
    d2Shape *shape = nullptr;

    d2Body* prev = nullptr;
    d2Body* next = nullptr;

    d2Body(const d2Shape &shape, float x, float y, float mass);

    ~d2Body();

    bool IsStatic() const;

    void ComputeAABB();

    void AddForce(const d2Vec2 &force);

    void AddTorque(float torque);

    void ClearForces();

    void ClearTorque();

    d2Vec2 LocalSpaceToWorldSpace(const d2Vec2 &point) const;

    d2Vec2 WorldSpaceToLocalSpace(const d2Vec2 &point) const;

    void ApplyImpulseLinear(const d2Vec2 &j);

    void ApplyImpulseAngular(const float j);

    void ApplyImpulseAtPoint(const d2Vec2 &j, const d2Vec2 &r);

    void IntegrateLinear(float dt);

    void IntegrateAngular(float dt);

    void IntegrateForces(const float dt);

    void IntegrateVelocities(const float dt);

    inline AABB* GetAABB() const { return aabb; }
};

#endif
