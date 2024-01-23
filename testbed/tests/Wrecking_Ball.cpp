#include "Test.h"

class Wrecking_Ball : public Test
{
public:
    Wrecking_Ball()
    {
        const int32 screenWidth = g_draw.screenWidth;
        const int32 screenHeight = g_draw.screenHeight;

        // create walls
        world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, 5.0f}, 0.0f);
        world->CreateBody(d2BoxShape(10.0f, screenHeight), {5.0f, screenHeight / 2.0f}, 0.0f);
        world->CreateBody(d2BoxShape(10.0f, screenHeight), {screenWidth - 5.0f, screenHeight / 2.0f}, 0.0f);
        d2Body *floor = world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, screenHeight - 5.0f}, 0.0f);

        // box pyramid
        int numRows = 5;
        for (int col = 0; col < numRows; col++)
        {
            for (int row = 0; row < col; row++)
            {
                float x = (floor->GetPosition().x) + col * 50.0f - (row * 30.0f);
                float y = (floor->GetPosition().y - 30.0f) - row * 52.0f;
                float mass = (5.0f / (row + 1.0f));

                d2Body *box = world->CreateBody(d2BoxShape(50, 50), (d2Vec2) {x, y}, mass);
                box->SetRestitution(0.f);
                box->SetFriction(0.3f);
            }
        }

        // Wrecking ball anchor
        d2Body *anchor = world->CreateBody(d2BoxShape(5.f, 5.f),
                                           (d2Vec2) {screenWidth / 2.0f, screenHeight / 2.0f - 100.0f},
                                           0.0f);

        // Create wrecking ball
        float ballRadius = 50.0f;
        float x = anchor->GetPosition().x - (0.35f * screenWidth);  // place the ball to the left of the chain based on screen width
        float y = anchor->GetPosition().y;
        d2Body *wreckingBall = world->CreateBody(d2CircleShape(ballRadius), (d2Vec2) {x, y}, 100.0f);

        // Create distance constraint between the anchor and the wrecking ball
        world->CreateJoint(anchor, wreckingBall, anchor->GetPosition());
    }

    void
    Input() override
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            auto mousePos = GetMousePosition();
            selectedBody = world->broadphase->Pick((d2Vec2) {mousePos.x, mousePos.y});
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            auto mousePos = GetMousePosition();
            world->CreateBody(d2CircleShape(25.0f), {mousePos.x, mousePos.y}, 1.0f);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            selectedBody = nullptr;
        }

        if (selectedBody)
        {
            auto mousePos = GetMousePosition();
            selectedBody->SetPosition((d2Vec2) {mousePos.x, mousePos.y});
        }
    }

    void
    Render() override
    {
        Test::Render();

        // Draw instructions
        DrawText("Left click to move the wrecking ball", g_draw.screenWidth - 400, 10, 20, WHITE);
        DrawText("Right click to create a new ball", g_draw.screenWidth - 350, 30, 20, WHITE);
    }

private:
    d2Body *selectedBody = nullptr;

public:
    static Test *Create() { return new Wrecking_Ball; }
};

static int testIndex = RegisterTest("Examples", "Wrecking Ball", Wrecking_Ball::Create);