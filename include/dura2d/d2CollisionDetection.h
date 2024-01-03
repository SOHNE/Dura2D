#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "d2Body.h"
#include "d2Contact.h"

struct d2CollisionDetection
{
    static bool IsColliding(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts);

    static bool IsCollidingCircleCircle(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts);

    static bool IsCollidingPolygonPolygon(d2Body *a, d2Body *b, std::vector<d2Contact> &contacts);

    static bool IsCollidingPolygonCircle(d2Body *polygon, d2Body *circle, std::vector<d2Contact> &contacts);
};

#endif
