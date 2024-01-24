#ifndef TEST_H
#define TEST_H

#include "dura2d/dura2d.h"
#include "draw.h"
#include "raylib.h"

class Test
{
public:
    Test();

    virtual ~Test();

    virtual void Step();
    virtual void Input();
    virtual void Render();

    virtual void Destroy() {}

public:
    d2World *world{nullptr};
};

typedef Test* TestCreateFcn();

int RegisterTest(const char* category, const char* name, TestCreateFcn* fcn);

//
struct TestEntry
{
    const char* category;
    const char* name;
    TestCreateFcn* createFcn;
};

#define MAX_TESTS 256
extern TestEntry g_availableTests[MAX_TESTS];
extern int g_testCount;

#endif //TEST_H
