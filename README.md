# Dura2D

<div align="center">

![Dura2D](docs/assets/logo.svg)

_An educational 2D Game Physics Engine_

<br />

[![MacOS Status](https://github.com/SOHNE/Dura2D/workflows/MacOS/badge.svg)](https://github.com/SOHNE/Dura2D/actions)
[![Windows Status](https://github.com/SOHNE/Dura2D/workflows/Windows/badge.svg)](https://github.com/SOHNE/Dura2D/actions)
[![Ubuntu Status](https://github.com/SOHNE/Dura2D/workflows/Ubuntu/badge.svg)](https://github.com/SOHNE/Dura2D/actions)
[![Install Status](https://github.com/SOHNE/Dura2D/workflows/Install/badge.svg)](https://github.com/SOHNE/Dura2D/actions)

[![GitHub release (latest by date)](https://img.shields.io/github/v/release/SOHNE/Dura2D?style=flat)](https://github.com/SOHNE/Dura2D/releases/latest)
[![License](https://img.shields.io/github/license/SOHNE/Dura2D?style=flat)](https://github.com/SOHNE/Dura2D/blob/main/LICENSE)
[![GitHub stars](https://img.shields.io/github/stars/SOHNE/Dura2D?style=social)](https://github.com/SOHNE/Dura2D/stargazers)

[Live Demo](https://dura2d.glitch.me/) | [Documentation](https://sohne.github.io/Dura2D) | [Report Bug](https://github.com/SOHNE/Dura2D/issues)

<div class="title_screenshot">

  ![GIF of Dura2D Test Bed](res/dura2d.gif)

</div>

</div>

## 🚀 Introduction

This 2D physics engine is an educational tool for exploring the principles behind game physics simulation. The project provides a hands-on approach to understanding fundamental 2D physics concepts, drawing inspiration from sources such as:

- [Pikuma's Game Physics Engine Programming course](https://pikuma.com/courses/game-physics-engine-programming)
- [Ming-Lun "Allen" Chou's Game Physics Series](https://allenchou.net/game-physics-series/)
- [Erin Catto's Box2D](https://box2d.org/)
- [Jason Gregory's Game Engine Architecture](https://www.gameenginebook.com/)

## 🌟 Features

- **Robust Constraint System**: Experience realistic physical interactions through constraint solver.
- **Warm Starting**: Enjoy faster simulations with our implementation of warm starting techniques.
- **Dynamic AABB Tree**: Enhances performance by efficiently updating bounding volume hierarchies in real-time during simulations.

## 📦 Installation

### CPM.cmake (Recommended)

This method is recommended for its simplicity and ease of version management.

1. First, set up [CPM.cmake] in your project:

   ```bash
   mkdir -p cmake
   wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
   ```

2. In your `CMakeLists.txt`, add the following lines after `project(...)`:

   ```cmake
   include(cmake/CPM.cmake)

   CPMAddPackage("gh:SOHNE/Dura2D@0.1.0")

   add_executable(your_target main.cpp)
   target_link_libraries(your_target PRIVATE Dura2D::Dura2D)
   ```

### Vendored

Choose one of the following vendored approaches based on your preference:

#### Git Submodule

1. Add Dura2D as a submodule to your project:
   ```bash
   git submodule add https://github.com/SOHNE/Dura2D.git vendor/Dura2D
   git submodule update --init --recursive
   ```

2. In your project's `CMakeLists.txt`, add the submodule directory:
   ```cmake
   add_subdirectory(vendor/Dura2D)
   ```

#### Git Subtree

If you prefer to have more control over updates and want to include Dura2D directly in your repository:

1. Add Dura2D as a subtree to your project:
   ```bash
   git subtree add --prefix vendor/Dura2D https://github.com/SOHNE/Dura2D.git main --squash
   ```

2. To update Dura2D in the future:
   ```bash
   git subtree pull --prefix vendor/Dura2D https://github.com/SOHNE/Dura2D.git main --squash
   ```

3. In your project's `CMakeLists.txt`, add the subtree directory:
   ```cmake
   add_subdirectory(vendor/Dura2D)
   ```

For both vendored approaches, link your target with Dura2D:
```cmake
target_link_libraries(your_target PRIVATE Dura2D)
```
## 🎮 Usage

Here's a quick example:

```cpp
#include <cstdio>
#include "dura2d/dura2d.h"

int
main()
{
    // Set up the world with gravity
    d2Vec2 gravity(0.F, -9.8F);
    d2World world(gravity);

    // Create a circular body (Shape, Position, Mass)
    d2Body * pBody = world.CreateBody(d2CircleShape(45), {0, 0}, 10.F);
    pBody->SetAngularVelocity(10.F);

    // Time step for the simulation
    constexpr float timeStep = 1.F / 60.F;

    // Simulate the physics for 5 seconds
    for (int i = 0; i < 60 * 5; ++i)
    {
        // Update the world for the next time step
        world.Step(timeStep);

        // Print the position and angle of the body every second
        if (i % 60 == 0)
        {
            d2Vec2 position = pBody->GetPosition();
            float  angle    = pBody->GetRotation();

            printf("Position: (%.2f, %.2f) | Angle: %.2f\n", position.x, position.y, angle);
        }
    }

    return 0;
}
```

## ⚒️ Building

### Prerequisites
- [CMake] 3.26+
- For [test bed web](#web-builds-for-testbed) builds: [Emscripten]/EMSDK properly set up in your environment

### Cloning the Repository
```bash
git clone https://github.com/SOHNE/Dura2D.git
cd Dura2D
```

### Dura2D Library

1. Configure the lib project:

  ```bash
  cmake -S. --preset <debug  |  release>
  ```

2. Build the project:
   ```bash
	cmake --build build/lib/<debug | release>
   ```

#### CMake Options

- `BUILD_SHARED_LIBS`: Build Dura2D as a shared library (default: OFF)
- `USE_CCACHE`: Enable compiler cache to improve build times (default: ON for top-level builds)

  Example configuration:
  ```bash
  cmake .. -DBUILD_SHARED_LIBS=ON -DUSE_CCACHE=ON
  ```

### Testbed

1. Configure the testbed project:
   ```bash
   cmake -S testbed --preset <debug | release>
   ```

2. Build the testbed:
   ```bash
   cmake --build build/testbed/<debug | release>
   ```

#### Web Builds for Testbed

For [Emscripten]/HTML5 builds:

1. Configure:
   ```bash
   cmake -S testbed --preset <web-debug | web-release>
   ```

2. Build:
   ```bash
   cmake --build build/testbed/<web-debug | web-release>
   ```


### Running Unit Tests

1. Configure the unit tests:
	  ```bash
	   cmake -S unit-test --preset <default  |  with-coverage  |  installed-version>
	 ```

2. Build the tests:
   ```bash
   cmake --build build/unit-test/<default | with-coverage | installed-version>
   ```

3. Run the tests:
   ```bash
   ctest --test-dir build/unit-test/<default | with-coverage | installed-version>
   ```

### Generating Documentation

1. Configure the documentation build:
   ```bash
   cmake -S docs --preset gendocs
   ```

2. Generate the documentation:
   ```bash
   cmake --build build/doc
   ```

3. The generated documentation will be available at:
   ```
   build/doc/doxygen/html/index.html
   ```

### Additional Notes

- All build artifacts are organized in the `build` directory, with subdirectories for each component (*main library*, *testbed*, *unit tests*, and *documentation*).
- Use the `--preset` option with `cmake` commands to easily switch between different configurations.
- For any build issues, ensure all [prerequisites] are correctly installed and environment variables (like EMSDK for web/HTML5 builds) are properly set.

## 🛣️ Roadmap

- [ ] Introduce contact caching for optimization
- [ ] Develop an island state system for bodies
- [ ] Add an awake state for bodies
- [ ] Create a robust manifold implementation

## 🤝 Contributing

We welcome contributions! Whether it's bug fixes, feature additions, or documentation improvements, your input is valuable.

## 🙏 Acknowledgements

A huge thank you to the open-source community and the authors of the resources that inspired this project. Your work continues to educate and inspire.

[//]: (Externals)
[CPM.cmake]: https://github.com/cpm-cmake/CPM.cmake/
[CMake]: https://cmake.org/
[Emscripten]: https://emscripten.org/

[prerequisites]: #prerequisites

[//]: (EOF)
