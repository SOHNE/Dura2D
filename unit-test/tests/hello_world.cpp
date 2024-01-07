#include "dura2d/d2World.h"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h"
#include <cstdio>

DOCTEST_TEST_CASE("hello world")
{
    d2Vec2 gravity(0.0F, 9.8F);
    auto *world = new d2World(gravity);

    d2Body *pBody = world->CreateBody(d2CircleShape(45), {0, 0}, 10.0F);
    pBody->angularVelocity = 10.0F;

    constexpr float timeStep = 1.F / 60.F;

    d2Vec2 position {};
    float angle {};

    // game loop
    for (int32_t i = 0; i < 60; ++i)
    {
        // Print the position and angle of the body.
        position = pBody->position;
        angle = pBody->rotation;
        printf("X: %4.2f | Y: %4.2f | Angle: %4.2f\n", position.x, position.y, angle);

        // Instruct the world to perform a single step of simulation.
        world->Update(timeStep);
    }


    // ##### TESTS #####
    CHECK( (position.y < 0.01F) );
    CHECK( (angle > 9.0F) );

    delete world;
}