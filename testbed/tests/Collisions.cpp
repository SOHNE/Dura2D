#include "Test.h"

class Collisions : public Test
{
public:
    d2Body *movingBody{nullptr};

    Collisions()
    {
        int32 screenWidth = g_draw.screenWidth;
        int32 screenHeight = g_draw.screenHeight;

        // Walls
        m_world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, 5.0f}, 0.0f);
        m_world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, screenHeight - 5.0f}, 0.0f);
        m_world->CreateBody(d2BoxShape(10.0f, screenHeight), {5.0f, screenHeight / 2.0f}, 0.0f);
        m_world->CreateBody(d2BoxShape(10.0f, screenHeight), {screenWidth - 5.0f, screenHeight / 2.0f}, 0.0f);

        // create a big box on the left
        movingBody = m_world->CreateBody(d2BoxShape(50.0f, 50.0f), {50.F, screenHeight / 2.0f}, 10.0f);
        movingBody->SetAngularVelocity(5.0F);
        movingBody->SetRestitution(1.0f);
        movingBody->SetGravityScale(0.f);

        float s = 7.0f; // Define your spacing here
        for (int i = 0; i < 150; ++i) {
            d2Body *body = m_world->CreateBody(d2CircleShape(5),
                                               {(float) (screenWidth / 2.0F) + (i % 10) * (5 + s),
                                                (movingBody->GetPosition().y / 1.25f) + (i / 10) * (5 + s)}, 0.01F);
            body->SetRestitution(1.0f);
            body->SetGravityScale(0.f);
        }
    }

    void Step(const float& dt, Settings& settings) override
    {
        Test::Step(dt, settings);

    movingBody->AddForce( (d2Vec2){2500, 0} );
    }

    static Test* Create()
    {
        return new Collisions;
    }
};

static int testIndex = RegisterTest("Examples", "Collisions", Collisions::Create);
