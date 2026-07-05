#include <doctest/doctest.h>

#include "dura2d/dura2d.h"

// A stack of boxes on a static floor must settle, fall asleep, and wake on
// contact
DOCTEST_TEST_CASE( "box stack settles and sleeps" )
{
    d2World world( d2Vec2( 0.0f, 9.81f ) );

    world.CreateBody( d2BoxShape( 800.0f, 20.0f ), { 400.0f, 10.0f },
                      0.0f ); // static floor

    constexpr int stackSize = 3;
    d2Body *      boxes[stackSize];
    for( int i = 0; i < stackSize; ++i )
        {
            boxes[i] = world.CreateBody( d2BoxShape( 50.0f, 50.0f ), { 400.0f, 50.0f + i * 55.0f }, 1.0f );
        }

    constexpr float dt = 1.0f / 60.0f;
    for( int i = 0; i < 60 * 5; ++i )
        {
            world.Step( dt, 8 );
        }

    // The stack settled: each box is above the floor and below its spawn height +
    // slack
    for( int i = 0; i < stackSize; ++i )
        {
            const float y = boxes[i]->GetPosition().y;
            CHECK( y > 20.0f + i * 45.0f );
            CHECK( y < 60.0f + i * 55.0f );
            CHECK( d2Abs( boxes[i]->GetPosition().x - 400.0f ) < 10.0f ); // no lateral drift
        }

    // The stack fell asleep
    for( int i = 0; i < stackSize; ++i )
        {
            CHECK( !boxes[i]->IsAwake() );
        }

    // A body dropped onto the sleeping stack wakes it
    d2Body * dropped = world.CreateBody( d2BoxShape( 50.0f, 50.0f ), { 400.0f, 400.0f }, 1.0f );
    bool     topWoke = false;
    for( int i = 0; i < 60 * 2 && !topWoke; ++i )
        {
            world.Step( dt, 8 );
            topWoke = boxes[stackSize - 1]->IsAwake();
        }
    CHECK( topWoke );
    CHECK( dropped != nullptr );
}
