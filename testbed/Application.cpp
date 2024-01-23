#include "Application.h"

#include <algorithm>
#include <unordered_set>

static int32 s_selectedTest = 0;
static Test *s_test = nullptr;

Application::Application()
{
    InitWindow(screenWidth, screenHeight, "Dura2D");
    SetTargetFPS(fps);

    IMGUI_CHECKVERSION();
    rlImGuiSetup(false);
    io = &ImGui::GetIO();

    _isRunning = true;

    SortTests();
    s_test = g_availableTests[s_selectedTest].createFcn();
}

Application::~Application()
{
    delete s_test;
    s_test = nullptr;
}

void
Application::Run()
{
    while (IsRunning()) {
        Input();
        Update();
        Render();
    }
}

void
Application::Input()
{
    if (IsKeyPressed(KEY_R)) {
        delete s_test;
        s_test = g_availableTests[s_selectedTest].createFcn();
        isPaused = false;
    }

    if (io->WantCaptureMouse) return;

    s_test->Input();
}

void
Application::Update()
{
    if (isPaused) return;

    s_test->Step();
}

void
Application::Render()
{
    BeginDrawing();

    s_test->Render();

    // rlimgui
    {
        const float deltaTime = GetFrameTime();
        const float frameTimeInMs = GetFrameTime() * 1000.0f;

        // start ImGui Conent
        rlImGuiBegin();

        // Draw FPS sampler
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Tools", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
        fps_values[fps_values_offset] = 1.0f / deltaTime;
        fps_values_offset = (fps_values_offset + 1) % IM_ARRAYSIZE(fps_values);
        char buffer[50];
        sprintf(buffer, "FPS:\n%.2f", 1.0f / deltaTime);

        ImGui::PlotLines(buffer, fps_values, IM_ARRAYSIZE(fps_values), fps_values_offset, nullptr, 0.0f, 240.0f,
                         ImVec2(0, 35));

        frametime_values[frametime_values_offset] = frameTimeInMs;
        frametime_values_offset = (frametime_values_offset + 1) % IM_ARRAYSIZE(frametime_values);
        sprintf(buffer, "Frame Time:\n%.2fms", frameTimeInMs);
        ImGui::PlotLines(buffer, frametime_values, IM_ARRAYSIZE(frametime_values), frametime_values_offset, nullptr,
                         0.0f, 100.0f, ImVec2(0, 35));

        // Change FPS
        if (ImGui::SliderInt("FPS", &fps, 24, 240)) {
            SetTargetFPS(fps);
        }

        // Bodies count
        ImGui::Text("Bodies: %d", s_test->world->GetBodyCount());
        ImGui::Text("Joints: %d", s_test->world->GetConstraintCount());

        // Pause/Resume button (use font awesome)
        if (isPaused) {
            if (ImGui::Button(ICON_FA_PLAY)) {
                isPaused = false;
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("Resume");
            }
        } else {
            if (ImGui::Button(ICON_FA_PAUSE)) {
                isPaused = true;
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("Pause");
            }
        }

        ImGui::SameLine();

        // Step button (use font awesome)
        if (ImGui::Button(ICON_FA_RIGHT_TO_BRACKET)) {
            isPaused = true;
            s_test->Step();
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Step forward");
        }

        ImGui::SameLine();

        // Reset button (use font awesome)
        if (ImGui::Button(ICON_FA_ROTATE)) {
            delete s_test;
            s_test = g_availableTests[s_selectedTest].createFcn();
            isPaused = false;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Reset");
        }

        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Controls")) {
                // Flags setter
                if (ImGui::CheckboxFlags("Shapes", &g_draw.m_flags, d2Draw::e_shapeBit)) {
                    g_draw.SetFlags(g_draw.m_flags);
                }
                if (ImGui::CheckboxFlags("AABBs", &g_draw.m_flags, d2Draw::e_aabbBit)) {
                    g_draw.SetFlags(g_draw.m_flags);
                }
                if (ImGui::CheckboxFlags("Transforms", &g_draw.m_flags, d2Draw::e_transformBit)) {
                    g_draw.SetFlags(g_draw.m_flags);
                }
                if (ImGui::CheckboxFlags("Joints", &g_draw.m_flags, d2Draw::e_jointBit)) {
                    g_draw.SetFlags(g_draw.m_flags);
                }
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Scenes")) {
                ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                               ImGuiTreeNodeFlags_DefaultOpen;

                std::unordered_set<std::string> processedCategories;

                for (int i = 0; i < g_testCount; ++i) {
                    std::string category = g_availableTests[i].category;
                    if (processedCategories.find(category) != processedCategories.end()) {
                        continue;
                    }

                    processedCategories.insert(category);

                    if (ImGui::TreeNodeEx(category.c_str(), nodeFlags)) {
                        for (int j = 0; j < g_testCount; ++j) {
                            if (strcmp(g_availableTests[j].category, g_availableTests[i].category) == 0) {
                                if (ImGui::Selectable(g_availableTests[j].name)) {
                                    s_selectedTest = j;
                                    delete s_test;
                                    s_test = g_availableTests[j].createFcn();
                                }
                            }
                        }
                        ImGui::TreePop();
                    }
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();

        // end ImGui Content
        rlImGuiEnd();
    }

    EndDrawing();
}

void
Application::Destroy()
{
    rlImGuiShutdown();
    CloseWindow();
}

inline bool
Application::CompareTests(const TestEntry &a, const TestEntry &b)
{
    int result = strcmp(a.category, b.category);
    if (result == 0) {
        result = strcmp(a.name, b.name);
    }

    return result < 0;
}

void
Application::SortTests()
{
    std::sort(g_availableTests, g_availableTests + g_testCount, CompareTests);
}
