#ifndef DURA2D_APPLICATION_H
#define DURA2D_APPLICATION_H

#include <iostream>
#include <cstdint>

#include "dura2d/d2World.h"


class Application
{
public:
    Application() = default;
    ~Application() = default;

    uint8_t _isRunning : 1 { false };


    inline bool IsRunning() const { return _isRunning; }

    void Setup();

    void Run();

    void Input();
    void Update();
    void Render();

    void Destroy();

    void DrawObject(const std::shared_ptr<d2Body>& body);

private:
    float rotation = 0.0f;

protected:
    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 600;

    const std::unique_ptr<d2World> world {std::make_unique<d2World>(-9.8F) };

    //d2Body* player {nullptr};
};


#endif //DURA2D_APPLICATION_H
