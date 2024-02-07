#include "Application.h"

#include <algorithm>
#include <unordered_set>

#include "settings.h"

static int32 s_selectedTest = 0;
static Test *s_test = nullptr;

Application::Application()
{
    m_settings = new Settings();

    InitWindow(screenWidth, screenHeight, "Dura2D");
    SetTargetFPS(m_settings->targetFPS);

    IMGUI_CHECKVERSION();
    rlImGuiSetup(false);
    m_io = &ImGui::GetIO();

    m_isRunning = true;

    SortTests();
    s_test = g_availableTests[s_selectedTest].createFcn();
}

Application::~Application()
{
    delete s_test;
    s_test = NULL;

    delete m_settings;
    m_settings = NULL;
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
    if (IsKeyPressed(KEY_F1)) {
        m_showUI = !m_showUI;
    }

    if (IsKeyPressed(KEY_P)) {
        TogglePause();
    }

    if (IsKeyPressed(KEY_S)) {
        m_isPaused = true;
        s_test->Step(*m_settings);
    }

    if (IsKeyPressed(KEY_R)) {
        delete s_test;
        s_test = g_availableTests[s_selectedTest].createFcn();
        m_isPaused = false;
    }

    if (m_io->WantCaptureMouse) return;

    s_test->Input();
}

void
Application::Update()
{
    if (m_isPaused) return;

    s_test->Step(*m_settings);
}

void
Application::Render()
{
    BeginDrawing();

    s_test->Render();

    // rlimgui
    if (m_showUI) {
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
        if (ImGui::SliderInt("FPS", &m_settings->targetFPS, 24, 240)) {
            SetTargetFPS(m_settings->targetFPS);
        }

        // Change position iterations
        if (ImGui::SliderInt("Position Iterations", &m_settings->positionIterations, 1, 10)) {
            m_settings->positionIterations = m_settings->positionIterations;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("Higher values can improve stability but decrease performance");
        }

        // Bodies count
        ImGui::Text("Bodies: %d", s_test->m_world->GetBodyCount());
        ImGui::SameLine();
        ImGui::Text("Joints: %d", s_test->m_world->GetConstraintCount());

        // Pause/Resume button (use font awesome)
        if (m_isPaused) {
            if (ImGui::Button(ICON_FA_PLAY)) {
                m_isPaused = false;
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("[P]lay/Resume");
            }
        } else {
            if (ImGui::Button(ICON_FA_PAUSE)) {
                m_isPaused = true;
            }
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::SetTooltip("[P]ause");
            }
        }

        ImGui::SameLine();

        // Step button (use font awesome)
        if (ImGui::Button(ICON_FA_RIGHT_TO_BRACKET)) {
            m_isPaused = true;
            s_test->Step(*m_settings);
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("[S]tep forward");
        }

        ImGui::SameLine();

        // Reset button (use font awesome)
        if (ImGui::Button(ICON_FA_ROTATE)) {
            delete s_test;
            s_test = g_availableTests[s_selectedTest].createFcn();
            m_isPaused = false;
        }
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
            ImGui::SetTooltip("[R]eset");
        }

        if (ImGui::BeginTabBar("TabBar")) {
            if (ImGui::BeginTabItem("Controls")) {
                // Flags setter
                if (ImGui::CheckboxFlags("Shapes", &g_draw.m_flags, d2Draw::e_shapeBit)) {
                    g_draw.SetFlags(g_draw.m_flags);
                }
                if (ImGui::CheckboxFlags("Polygon Meshes", &g_draw.m_flags, d2Draw::e_meshBit)) {
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

        s_test->DrawUI();

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
