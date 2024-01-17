#include "dura2d/d2World.h"
#include "dura2d/d2Constants.h"
#include "dura2d/d2CollisionDetection.h"

d2World::d2World(const d2Vec2 &gravity)
{
    m_gravity = gravity * -1.0f;
    broadphase = std::make_unique<d2NSquaredBroad>();
    m_blockAllocator = d2BlockAllocator();
}

d2World::~d2World()
{
    // delete all m_bodiesList
    for (auto body = m_bodiesList; body; body = body->next) {
        body->~d2Body();
        m_blockAllocator.Free(body, sizeof(d2Body));
    }
    m_bodiesList = nullptr;

    constraints.clear();
    broadphase.reset();
}

d2Body*
d2World::CreateBody(const d2Shape &shape, d2Vec2 position, float mass)
{
    void* ptr = m_blockAllocator.Allocate(sizeof(d2Body));
    d2Body* body = new(ptr) d2Body(shape, position.x, position.y, mass, this);

    // Add to world doubly linked list.
    body->prev = nullptr;
    body->next = m_bodiesList;
    if (m_bodiesList) {
        m_bodiesList->prev = body;
    }
    m_bodiesList = body;
    ++m_bodyCount;

    // add to broadphase
    broadphase->Add(body);

    return body;
}

void
d2World::AddConstraint(d2Constraint *constraint)
{
    constraints.push_back(constraint);
}

std::vector<d2Constraint *> &
d2World::GetConstraints()
{
    return constraints;
}

void
d2World::AddForce(const d2Vec2 &force)
{
    forces.push_back(force);
}

void
d2World::AddTorque(float torque)
{
    torques.push_back(torque);
}

void
d2World::Update(float dt)
{
    // Create a vector of penetration constraints that will be solved frame per frame
    static std::vector<d2PenetrationConstraint> penetrations {};

    penetrations.clear();

    // Loop all m_bodiesList of the world applying forces
    for (auto body = m_bodiesList; body; body = body->next) {
        // Apply the weight force to all m_bodiesList
        d2Vec2 weight = d2Vec2(1.0, body->mass * PIXELS_PER_METER) * m_gravity;
        body->AddForce(weight);

        // Apply forces to all m_bodiesList
        for (auto& force: forces)
            body->AddForce(force);

        // Apply torque to all m_bodiesList
        for (auto& torque: torques)
            body->AddTorque(torque);

        body->IntegrateForces(dt);
        body->ComputeAABB();
    }

    {
        auto &pairs = broadphase->ComputePairs();
        for (const auto &pair: pairs) {
            auto a = pair.first;
            auto b = pair.second;

            std::vector<d2Contact> contacts{};
            if (!d2CollisionDetection::IsColliding(a, b, contacts)) continue;

            for (const auto &contact: contacts) {
                // Create a new penetration constraint
                d2PenetrationConstraint penetration(contact.a, contact.b, contact.start, contact.end, contact.normal);
                penetrations.push_back(penetration);
            }
        }
    }

    // Solve all constraints
    for (auto &constraint: constraints) {
        constraint->PreSolve(dt);
    }
    for (auto &constraint: penetrations) {
        constraint.PreSolve(dt);
    }
    for (int i = 0; i < 5; i++)
    {
        for (auto &constraint: constraints) {
            constraint->Solve();
        }
        for (auto &constraint: penetrations) {
            constraint.Solve();
        }
    }
    for (auto &constraint: constraints) {
        constraint->PostSolve();
    }
    for (auto &constraint: penetrations) {
        constraint.PostSolve();
    }

    // Integrate all the velocities
    for (auto body = m_bodiesList; body; body = body->next) {
        body->IntegrateVelocities(dt);
    }
}
