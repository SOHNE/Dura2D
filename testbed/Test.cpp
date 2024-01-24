#include "Test.h"

#include <iostream>

Test::Test()
{
    world = new d2World((d2Vec2){0.0f, -9.81f});
    world->SetDebugDraw(&g_draw);
}

Test::~Test()
{
    delete world;
    world = NULL;
    Destroy();
}

void
Test::Step()
{
    world->Update(GetFrameTime());
}

void
Test::Input()
{

}

void
Test::Render()
{
    ClearBackground((Color){45, 45, 48, 255});
    world->DebugDraw();
}

TestEntry g_availableTests[MAX_TESTS] = { {nullptr} };
int g_testCount = 0;

int RegisterTest(const char* category, const char* name, TestCreateFcn* fcn)
{
    int index = g_testCount;
    if (index >= MAX_TESTS) return -1;

    g_availableTests[index] = {category, name, fcn };
    ++g_testCount;
    return index;
}
