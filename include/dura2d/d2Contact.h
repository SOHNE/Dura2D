#ifndef CONTACT_H
#define CONTACT_H

#include "dura2d/d2api.h"
#include "d2Vec2.h"
#include "d2Body.h"

struct D2_API d2Contact
{
    d2Body *a;
    d2Body *b;

    d2Vec2 start;
    d2Vec2 end;

    d2Vec2 normal;
    float depth;
};

#endif
