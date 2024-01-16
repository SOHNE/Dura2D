#include "Application.h"
#include "raylib.h"
#include "raygui.h"

void
Application::Setup()
{
    InitWindow(screenWidth, screenHeight, "Dura2D [raylib-testbed]");
    SetTargetFPS(60);
    _isRunning = true;

    // create a box in middle with 0 mass
    {
        d2Body *mBody = world->CreateBody(d2BoxShape(50.0f, 50.0f), {screenWidth / 2.0f, screenHeight / 2.0f}, 0.0f);
        mBody->SetAngularVelocity(0.5f);
    }

    // create ground
    {
        d2Body *bGround = world->CreateBody(d2BoxShape(screenWidth, 10.0f), {screenWidth / 2.0f, screenHeight - 5.0f},
                                            0.0f);
    }

    // Add walls
    {
        d2Body *bLeftWall = world->CreateBody(d2BoxShape(10.0f, screenHeight), {5.0f, screenHeight / 2.0f}, 0.0f);
        d2Body *bRightWall = world->CreateBody(d2BoxShape(10.0f, screenHeight),
                                               {screenWidth - 5.0f, screenHeight / 2.0f},
                                               0.0f);
    }

    // create 1000 boxes
    for (int i = 0; i < 10; ++i)
    {
        d2Body *body = world->CreateBody(d2BoxShape(10.0f, 10.0f), {(float) GetRandomValue(0, screenWidth),
                                                                    (float) GetRandomValue(0, screenHeight)}, 1.0f);
        body->SetRestitution(0.2f);
    }
}

void
Application::Run()
{
    while (!WindowShouldClose() && IsRunning()) {
        Input();
        Update();
        Render();
    }
}

void
Application::Input()
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

void
Application::Update()
{
    world->Update(GetFrameTime());
}

void
Application::Render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // draw body
    {
        auto bodies = world->GetBodies();
        for (auto &body = bodies; body; body = body->GetNext()) {
            DrawObject(body);
        }
    }

    // Draw FPS
    {
        DrawFPS(15, 10);

        auto frameTimeInMs = GetFrameTime() * 1000.0f;
        DrawText(TextFormat("Frame Time: %.2f ms", frameTimeInMs), 15, 30, 10, BLACK);

        auto bodyCount = world->GetBodyCount();
        DrawText(TextFormat("Body Count: %d", bodyCount), 15, 50, 10, BLACK);
    }

    EndDrawing();
}

void
Application::Destroy()
{
    CloseWindow();
}

void
Application::DrawObject(const d2Body *body)
{
    d2Shape *shape = body->GetShape();

    Color color = body->IsAwake() ? MAROON : DARKGRAY;

    switch (shape->GetType()) {
        case POLYGON:
        case BOX: {
            float rot = body->GetRotation() * RAD2DEG;
            auto *box = dynamic_cast<d2BoxShape *>(shape);
            DrawRectanglePro({body->GetPosition().x, body->GetPosition().y, box->width, box->height},
                             {box->width / 2.0f, box->height / 2.0f}, rot,
                             color
                            );
            break;
        }
        case CIRCLE: {
            auto *circle = dynamic_cast<d2CircleShape *>(shape);
            DrawCircle(body->GetPosition().x, body->GetPosition().y, circle->radius, color);
            break;
        }
        default:
            break;
    }
}
