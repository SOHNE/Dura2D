#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "../doctest.h"

#include <cstdio>
#include "dura2d/dura2d.h"

DOCTEST_TEST_CASE("hello world")
{
    // Set up the world
    d2Vec2 gravity(0.0F, -9.81F);
    d2World world(gravity);

    // Create a body
    d2Body *pBody = world.CreateBody(d2CircleShape(45), (d2Vec2){0.F, 0.F}, 10.0F);
    pBody->SetAngularVelocity(1.0F);

    // also known as delta time, or the time between frames
    constexpr float timeStep = 1.F / 60.F;

    d2Vec2 position {};
    float angle {};

    // game loop
    for (int32_t i = 0; i < 60; ++i)
    {
        // Print the position and angle of the body.
        position = pBody->GetPosition();    // in meters
        angle = pBody->GetRotation();       // in radians
        printf("%d - X: %4.2f | Y: %4.2f | Angle: %4.2f\n", i, position.x, position.y, angle);

        // Instruct the world to perform a single step of simulation.
        world.Step(timeStep);
    }

    // ##### TESTS #####
    CHECK( ( position.x == 0.0F ) );
    CHECK( ( position.y > 0.0F ) );
    CHECK( ( angle > 0.0F ) );
}