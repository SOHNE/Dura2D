# SOHNE | Dura2D

Dura2D is an educational 2D physics engine project developed alongside the [Pikuma's course] and [Allen Game Physics] blog series, aiming to provide a deep understanding of physics simulation principles.

Drawing inspiration from [Game Engine Architecture] by Jason Gregory, Dura2D focuses on learning and continuous development, offering features like a robust constraint system for realistic simulations, warm starting for improved simulation speed, and a stable performance foundation.

[![Live demo](https://img.shields.io/badge/-Live%20Demo-8f60fc?style=for-the-badge&logo=glitch&logoColor=white&colorA=392467&colorB=7E30E1)](https://dura2d.glitch.me/)

## Features

- **Constraint System**: A robust system that enables realistic simulations of physical interactions.
- **Warm Starting**: Improves the speed of the physics simulation by using results from previous frames to kick-start the current frame.
- **Stability**: The engine is designed with stability in mind and performs at a decent level.

## Installation

To install Dura2D, you'll need to have CMake version 3.15 or higher installed on your system. You can download it from the official [CMake] website.

Follow these steps:

1. Clone the repository: `git clone https://github.com/sohne/dura2d.git`
2. Navigate to the project directory: `cd dura2d`
3. Create a new directory for the build: `mkdir build && cd build`
4. Run CMake to configure the project: `cmake ..`
5. Build the project: `make`

## Usage

You can use the Dura2D library in your own projects for educational purposes, as it's designed to deepen your understanding of physics simulation.

However, please note that Dura2D is not intended for production use. It's a learning tool and may not be as robust or reliable as production-ready physics engines. Keep in mind that this project is purely educational and may contain bugs or limitations that could affect its functionality.

### Add Dura2D as a Subdirectory in Your Project

In your own project directory, create a CMakeLists.txt file if you don't already have one. Add the following lines to include Dura2D as a subdirectory in your project:

```cmake
add_subdirectory(path/to/dura2d)
target_link_libraries(your_project_name PRIVATE dura2d)
```
_Replace **path/to/dura2d** with the actual path to the Dura2D directory relative to your project._

### Use Dura2D in Your Code

You can now use Dura2D in your project by including its header files and linking against the Dura2D library. Here's an example of how you might use Dura2D in a C++ source file:

```cpp
#include <cstdio>
#include "dura2d/dura2d.h"

int
main()
{
    // Set up the world with gravity
    d2Vec2 gravity(0.0F, -9.8F);
    d2World world(gravity);

    // Create a circular body (Shape, Position, Mass)
    d2Body *pBody = world.CreateBody(d2CircleShape(45), {0, 0}, 10.0F);

    // Game loop
    {
        // Time step for the simulation
        constexpr float timeStep = 1.F / 60.F;

        // Position and angle of the body
        d2Vec2 position {};
        float angle {};
    
        // Simulate the physics for 60 frames
        for (int32_t i = 0; i < 60; ++i)
        {
            // Update the world for the next time step
            world.Update(timeStep);
            
            // Print the position and angle of the body
            position = pBody->position;
            angle = pBody->rotation;
            printf("Position: (%.2f, %.2f) | Angle: %.2f\n", position.x, position.y, angle);
        }
    }

    return 0;
}

```

## CMake Options

Dura2D provides several configuration options that you can set when running CMake. Here's a list of the available options:

- `DURA2D_BUILD_UNIT_TESTS`: Build the Dura2D unit tests. Default is `ON`.
- `DURA2D_BUILD_TESTBED`: Build the Dura2D testbed. Default is `ON`.
- `DURA2D_MACOSX_BUNDLE`: Build Dura2D as a macOS bundle. Default is `ON`.
- `BUILD_SHARED_LIBS`: Build Dura2D as a shared library. Default is `OFF`.

You can set these options when configuring the project with CMake. For example, to build the unit tests and the testbed, but not the macOS bundle or the shared library, you would run:

```bash
cmake .. -DDURA2D_BUILD_UNIT_TESTS=ON -DDURA2D_BUILD_TESTBED=ON -DDURA2D_MACOSX_BUNDLE=OFF -DBUILD_SHARED_LIBS=OFF
```

## Todos

- [ ] **Broadphase Collision Detection**: Improve the efficiency of the engine by reducing the number of collision checks.
- [ ] **Contact Caching**: Optimize the engine by storing collision information and reusing it in subsequent frames.
- [ ] **Memory Arena**: Implement a custom memory management system to optimize memory usage and performance.

[//]:  (Externals)
[Pikuma's course]: https://pikuma.com/courses/game-physics-engine-programming
[Allen Game Physics]: https://allenchou.net/game-physics-series/
[Game Engine Architecture]: https://www.gameenginebook.com/
[CMake]: https://cmake.org/
[//]:  (EOF)
