#include <cmath>

#include "Test.h"

#define TAU 6.28318530718f

class Playground : public Test
{
public:

    Playground()
    {
        int32 screenWidth = g_draw.screenWidth;
        int32 screenHeight = g_draw.screenHeight;

        // create a polygon in middle with 0 mass
        {
            int32 sideCount = 4;
            float radius = 50.0f;
            float angle = TAU / (sideCount + 1);
            d2Vec2 vertices[sideCount];
            for (int i = 0; i < sideCount; ++i) {
                vertices[i] = {radius * cosf(angle * i), radius * sinf(angle * i)};
            }

            d2Body *poly = world->CreateBody(d2PolygonShape(vertices, sideCount), {screenWidth / 2.0f, screenHeight / 2.0f}, 0.0f);
            poly->SetRestitution(0.2f);
            poly->SetFriction(0.0f);
        }

        // create walls
        {
            world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, 5.0f}, 0.0f);
            world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, screenHeight - 5.0f}, 0.0f);
            world->CreateBody(d2BoxShape(10.0f, screenHeight), {5.0f, screenHeight / 2.0f}, 0.0f);
            world->CreateBody(d2BoxShape(10.0f, screenHeight), {screenWidth - 5.0f, screenHeight / 2.0f}, 0.0f);
        }

        // create 1000 boxes
        for (int i = 0; i < 25; ++i) {
            d2Body *body = world->CreateBody(d2BoxShape(10.0f, 10.0f), {(float) GetRandomValue(0, screenWidth),
                                                                        (float) GetRandomValue(0, screenHeight)}, 1.0f);
            body->SetRestitution(0.2f);
        }
    }

    void Step() override
    {
        Test::Step();
    }

    void Input() override
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            auto mousePos = GetMousePosition();
            d2Body *body = world->CreateBody(d2BoxShape(50.0f, 50.0f), {mousePos.x, mousePos.y}, 1.0f);
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            auto mousePos = GetMousePosition();
            d2Body *body = world->CreateBody(d2CircleShape(25.0f), {mousePos.x, mousePos.y}, 1.0f);
        }
    }

    static Test* Create()
    {
        return new Playground;
    }
};

static int testIndex = RegisterTest("Examples", "Playground", Playground::Create);
