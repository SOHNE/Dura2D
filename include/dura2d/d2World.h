#ifndef WORLD_H
#define WORLD_H

#include "d2api.h"
#include "./d2Body.h"
#include "./d2Constraint.h"
#include "./dura2d/d2NSquaredBroad.h"
#include <vector>

class D2_API d2World
{
private:
    float G = 9.8;
    std::vector<std::shared_ptr<d2Body>>  bodies;
    std::vector<d2Constraint *> constraints;

    std::vector<d2Vec2> forces;
    std::vector<float> torques;

    std::unique_ptr<d2Broadphase> broadphase;

public:
    d2World(float gravity);

    ~d2World();

    void AddBody(const std::shared_ptr<d2Body>& body);

    std::vector<std::shared_ptr<d2Body>> & GetBodies();

    void AddConstraint(d2Constraint *constraint);

    std::vector<d2Constraint *> &GetConstraints();

    void AddForce(const d2Vec2 &force);

    void AddTorque(float torque);

    void Update(float dt);

    void CheckCollisions();
};

#endif
