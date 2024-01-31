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

        for (int i = 0; i < 150; ++i) {
            d2Body *body = m_world->CreateBody(d2CircleShape(5.0f),
                                               {(float) (screenWidth / 2.0F) + (i % 10) * 10,
                             (movingBody->GetPosition().y / 1.25F) + (i / 10) * 10}, 0.05F);
            body->SetRestitution(1.0f);
            body->SetGravityScale(0.f);
        }
    }

    void Step(Settings& settings) override
    {
        Test::Step(settings);

        movingBody->AddForce({2500, 0});
    }

    static Test* Create()
    {
        return new Collisions;
    }
};

static int testIndex = RegisterTest("Examples", "Collisions", Collisions::Create);
