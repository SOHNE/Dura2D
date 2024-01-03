#include "Application.h"
#include "raylib.h"
#include "raygui.h"

void
Application::Setup()
{
    InitWindow(screenWidth, screenHeight, "Dura2D [raylib-testbed]");
    SetTargetFPS(60);
    _isRunning = true;

    // add ground
    auto ground = std::make_shared<d2Body>(d2BoxShape(screenWidth, 50.0f), screenWidth / 2.0f, screenHeight - 25.0f, 0.0f);
    ground->restitution = 0.0f;
    world->AddBody(ground);

    // walls
    auto leftWall = std::make_shared<d2Body>(d2BoxShape(50.0f, screenHeight), 25.0f, screenHeight / 2.0f, 0.0f);
    auto rightWall = std::make_shared<d2Body>(d2BoxShape(50.0f, screenHeight), screenWidth - 25.0f, screenHeight / 2.0f, 0.0f);
    leftWall->restitution = 0.0f;
    rightWall->restitution = 0.0f;
    world->AddBody(leftWall);
    world->AddBody(rightWall);

    // Do a simple dominoe
    int dominoeCount = 7;
    float dominoeWidth = 15.0f;
    float dominoeHeight = 100.0f;
    float dominoeSpacing = 60.0f;
    float dominoeStartX = (screenWidth / 2.0f) - ((dominoeWidth + dominoeSpacing) * dominoeCount / 2.0f);
    float dominoeStartY = screenHeight - dominoeHeight / 2.0f - 50.0f;
    for (int i = 0; i < dominoeCount; ++i)
    {
        auto dominoe = std::make_shared<d2Body>(d2BoxShape(dominoeWidth, dominoeHeight),
                                              dominoeStartX + (i * (dominoeWidth + dominoeSpacing)),
                                                dominoeStartY, 100.0f);
        dominoe->restitution = 0.0f;
        world->AddBody(dominoe);
    }
}

void
Application::Run()
{
    while (!WindowShouldClose() && IsRunning())
    {
        Input();
        Update();
        Render();
    }
}

void
Application::Input()
{
    // on mouse click, add circle
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        auto circle = std::make_shared<d2Body>(d2CircleShape(30), GetMouseX(), GetMouseY(), 10.0);
        circle->restitution = 1.0f;
        world->AddBody(circle);
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        auto box = std::make_shared<d2Body>(d2BoxShape(30, 30), GetMouseX(), GetMouseY(), 10.0);
        box->restitution = 0.0f;
        world->AddBody(box);
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
        auto &bodies = world->GetBodies();
        for (const auto &body: bodies) {
            DrawObject(body);
        }
    }

    // Draw FPS
    {
        DrawFPS(10, 10);

        auto frameTimeInMs = GetFrameTime() * 1000.0f;
        DrawText(TextFormat("Frame Time: %.2f ms", frameTimeInMs), 10, 30, 10, BLACK);

        auto objectCount = world->GetBodies().size();
        DrawText(TextFormat("Object Count: %d", objectCount), 10, 40, 10, BLACK);
    }

    EndDrawing();
}

void
Application::Destroy()
{
    CloseWindow();
}

void
Application::DrawObject(const std::shared_ptr<d2Body>& body)
{
    d2Shape* shape = body->shape;
    switch (shape->GetType())
    {
        case POLYGON:
        case BOX:
        {
            float rot = body->rotation * RAD2DEG;
            auto* box = dynamic_cast<d2BoxShape*>(shape);
            DrawRectanglePro({ body->position.x, body->position.y, box->width, box->height },
                             { box->width / 2.0f, box->height / 2.0f }, rot,
                             Fade(MAROON, 0.3f)
            );
            break;
        }
        case CIRCLE:
        {
            auto* circle = dynamic_cast<d2CircleShape*>(shape);
            DrawCircle(body->position.x, body->position.y, circle->radius, Fade(MAROON, 0.3f));
            break;
        }
        default:
            break;
    }
}
