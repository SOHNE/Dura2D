#include "dura2d/d2World.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h"
#include <cstdio>

DOCTEST_TEST_CASE("hello world")
{
    auto *world = new d2World(9.8F);

    d2Body *pBody = new d2Body(d2CircleShape(45), 0, 0, 10.0);
    pBody->angularVelocity = 10.0F;
    world->AddBody(pBody);

    constexpr float timeStep = 1.0f / 60.0F;

    d2Vec2 position {};
    float angle {};

    // game loop
    for (int32_t i = 0; i < 60; ++i)
    {
        // Print the position and angle of the body.
        position = pBody->position;
        angle = pBody->rotation;
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

        // Instruct the world to perform a single step of simulation.
        world->Update(timeStep);
    }


    // ##### TESTS #####
    CHECK( (position.x < 0.01F) );
    CHECK( (angle > 9.0F) );

    delete world;
}