#ifndef FORCE_H
#define FORCE_H

#include "d2api.h"

class d2Body;
struct d2Vec2;

struct D2_API d2Force
{
    static d2Vec2 GenerateDragForce(const d2Body &body, float k);

    static d2Vec2 GenerateFrictionForce(const d2Body &body, float k);

    static d2Vec2 GenerateSpringForce(const d2Body &body, d2Vec2 anchor, float restLength, float k);

    static d2Vec2 GenerateGravitationalForce(const d2Body &a, const d2Body &b, float G, float minDistance, float maxDistance);
};

#endif
