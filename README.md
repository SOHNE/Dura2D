
# SOHNE | Dura2D

This is an educational physics engine project developed alongside the [Pikuma's course] and [Allen Game Physics] blog series. It's designed to provide a deep understanding of the core principles of physics simulation, with a focus on learning and continuous development.

### Features

- **Constraint System**: A robust system that allows for realistic simulations of physical interactions.
- **Warm Starting**: Improves the speed of the physics simulation by using the results from previous frames to kick-start the current frame.
- **Stability**: The engine has been designed with stability in mind, and it performs at a decent level.

## Installation

To install Dura2D, you need to have CMake version 3.15 or higher installed on your system. You can download it from the official CMake website.

Once you have CMake installed, follow these steps:

1. Clone the repository: `git clone https://github.com/sohne/dura2d.git`
2. Navigate to the project directory: `cd dura2d`
3. Create a new directory for the build: `mkdir build && cd build`
4. Run CMake to configure the project: `cmake ..`
5. Build the project: `make`

## Usage

You can use the Dura2D library in your own projects. Just include the `dura2d.h` header file and link against the Dura2D library.

Please note that the paths might vary depending on your system and CMake configuration.

## CMake Options

The Dura2D project provides several configuration options that you can set when running CMake. Here's a list of the available options:

- `DURA2D_BUILD_UNIT_TESTS`: Set this to `ON` to build the Dura2D unit tests. Default is `ON`.
- `DURA2D_BUILD_TESTBED`: Set this to `ON` to build the Dura2D testbed. Default is `ON`.
- `DURA2D_MACOSX_BUNDLE`: Set this to `ON` to build Dura2D as a macOS bundle. Default is `ON`.
- `BUILD_SHARED_LIBS`: Set this to `ON` to build Dura2D as a shared library. Default is `OFF`.

You can set these options when configuring the project with CMake. For example, to build the unit tests and the testbed, but not the macOS bundle or the shared library, you would run:

```bash
cmake .. -DDURA2D_BUILD_UNIT_TESTS=ON -DDURA2D_BUILD_TESTBED=ON -DDURA2D_MACOSX_BUNDLE=OFF -DBUILD_SHARED_LIBS=OFF
```
---

### Todos

- **Broadphase Collision Detection**: To improve the efficiency of the engine by reducing the number of collision checks.
- **Contact Caching**: To optimize the engine by storing collision information and reusing it in subsequent frames.
- **Memory Arena**: To implement a custom memory management system that optimizes memory usage and performance.

[//]:  (Externals)
[Pikuma's course]: https://pikuma.com/courses/game-physics-engine-programming
[Allen Game Physics]: https://allenchou.net/game-physics-series/
