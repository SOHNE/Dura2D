#include "Test.h"

class Bridge : public Test
{
public:
    Bridge()
    {
        int32 numPlanks = 9;
        int32 spacing = 35;
        float totalBridgeLength = (numPlanks + 4.f) * spacing;

        d2Body *startStep = world->CreateBody(d2BoxShape(80.0f, 20.0f),
                                              (d2Vec2) {g_draw.screenWidth / 2.0f - totalBridgeLength / 2.0f,
                                                        g_draw.screenHeight / 2.0f + 50}, 0.0f);
        d2Body *endStep = world->CreateBody(d2BoxShape(80.0f, 20.0f),
                                            (d2Vec2) {startStep->GetPosition().x + totalBridgeLength,
                                                      startStep->GetPosition().y}, 0.0f);

        d2Body *prevBodyStart = startStep;
        d2Body *prevBodyEnd = endStep;
        for (int32 i = 1; i <= (numPlanks / 2) + 1; ++i) {
            float xStart = startStep->GetPosition().x + spacing + (i * spacing);
            float yStart = startStep->GetPosition().y + 20;
            float xEnd = endStep->GetPosition().x - spacing - (i * spacing);
            float yEnd = endStep->GetPosition().y + 20;
            float mass = 3.0f;

            d2Body *bodyStart = world->CreateBody(d2CircleShape(15.0f), (d2Vec2) {xStart, yStart}, mass);
            d2Body *bodyEnd = world->CreateBody(d2CircleShape(15.0f), (d2Vec2) {xEnd, yEnd}, mass);
            world->CreateJoint(prevBodyStart, bodyStart, bodyStart->GetPosition());
            world->CreateJoint(prevBodyEnd, bodyEnd, bodyEnd->GetPosition());
            prevBodyStart = bodyStart;
            prevBodyEnd = bodyEnd;
        }
// Create joint between the two middle bodies
        world->CreateJoint(prevBodyStart, prevBodyEnd, prevBodyEnd->GetPosition());
    }

    void
    Input() override
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            auto mousePos = GetMousePosition();
            world->CreateBody(d2BoxShape(50.0f, 50.0f), (d2Vec2) {mousePos.x, mousePos.y}, 5.0f);
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            auto mousePos = GetMousePosition();
            d2Body *body = world->broadphase->Pick((d2Vec2) {mousePos.x, mousePos.y});
            if (body) world->DestroyBody(body);
        }
    }

    static Test *Create() { return new Bridge; }
};

static int testIndex = RegisterTest("Examples", "Bridge", Bridge::Create);