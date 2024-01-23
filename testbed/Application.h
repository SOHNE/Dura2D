#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <cstdint>

#include "dura2d/dura2d.h"
#include "raylib.h"
#include "Test.h"

#include "imgui.h"
#include "rlImGui.h"

// Forward declarations
class draw;
class Test;

class Application
{
public:
    Application();

    ~Application();

    void Run();

    void Input();
    void Update();
    void Render();

    void Destroy();

public:
    inline void TogglePause() { isPaused = !isPaused; }
    inline void SetPaused(bool paused) { isPaused = paused; }
    inline bool IsRunning() const { return !WindowShouldClose() && _isRunning; }

private:
    static inline bool CompareTests(const TestEntry& a, const TestEntry& b);
    static void SortTests();

private:
    float fps_values[180]{0};
    int32 fps_values_offset{0};

    float frametime_values[180]{0};
    int32 frametime_values_offset{0};

    int32 fps{60};
    uint8 _isRunning: 1 {false};
    uint8 isPaused: 1 {false};

    ImGuiIO* io {nullptr};

public:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 600;
};


#endif //APPLICATION_H
