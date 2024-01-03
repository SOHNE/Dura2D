#include "dura2d/d2World.h"
#include "dura2d/d2Constants.h"
#include "dura2d/d2CollisionDetection.h"

#include "dura2d/d2NSquaredBroad.h"

d2World::d2World(float gravity) : G(-gravity)
{
    broadphase = std::make_unique<d2NSquaredBroad>();
}

d2World::~d2World()
{
    for (auto body: bodies) {
        body.reset();
    }
    for (auto constraint: constraints) {
        delete constraint;
    }
    bodies.clear();
    constraints.clear();

    broadphase.reset();
}

void
d2World::AddBody(const std::shared_ptr<d2Body>& body)
{
    bodies.push_back(body);
    broadphase->Add(body);
}

std::vector<std::shared_ptr<d2Body>> &
d2World::GetBodies()
{
    return bodies;
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

    // Loop all bodies of the world applying forces
    for (auto &body: bodies) {
        // Apply the weight force to all bodies
        d2Vec2 weight = d2Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);

        // Apply forces to all bodies
        for (auto& force: forces)
            body->AddForce(force);

        // Apply torque to all bodies
        for (auto& torque: torques)
            body->AddTorque(torque);

        body->IntegrateForces(dt);
        body->ComputeAABB();
    }

    {
        auto &pairs = broadphase->ComputePairs();
        for (const auto &pair: pairs) {
            auto a = pair.first.get();
            auto b = pair.second.get();

            std::vector<d2Contact> contacts{};
            if (!d2CollisionDetection::IsColliding(a, b, contacts)) continue;

            for (auto &contact: contacts) {
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
    for (auto &body: bodies) {
        body->IntegrateVelocities(dt);
    }
}
