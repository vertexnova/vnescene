<p align="center">
  <img src="icons/vertexnova_logo_medallion_with_text.svg" alt="VertexNova Scene" width="320"/>
</p>

<p align="center">
  <strong>Scene, camera, and light building blocks for VertexNova</strong>
</p>

<p align="center">
  <a href="https://github.com/vertexnova/vnescene/actions/workflows/ci.yml">
    <img src="https://github.com/vertexnova/vnescene/actions/workflows/ci.yml/badge.svg?branch=main" alt="CI"/>
  </a>
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg" alt="C++ Standard"/>
  <a href="https://codecov.io/gh/vertexnova/vnescene">
    <img src="https://codecov.io/gh/vertexnova/vnescene/branch/main/graph/badge.svg" alt="Coverage"/>
  </a>
  <img src="https://img.shields.io/badge/license-Apache%202.0-green.svg" alt="License"/>
</p>

---

## About

VneScene provides view (cameras) and lighting data structures with GPU-friendly packing. It does not implement ECS, rendering, or input — it is designed for use by a renderer or viewer as part of the [VertexNova](https://github.com/vertexnova) stack.

VneScene is a C++20 library offering:

- **Cameras**: Perspective and orthographic cameras with view/projection matrices, GPU pack layout, fit-to-AABB, screen-to-world ray, and transform sync with external nodes.
- **Lights**: Ambient, directional, point, and spot lights with a common GPU layout and optional shadow settings.
- **SceneState**: One active camera plus a list of lights (with optional max count); suitable for sandbox and viewer use cases.
- **Handles**: Type-safe generational handles (`CameraId`, `LightId`) for identity.

It depends on **vnemath** for matrices, vectors, and rays. The example programs optionally use **vnelogging** for console output.

## Features

- **Cameras**: `ICamera`, `PerspectiveCamera`, `OrthographicCamera`, `CameraFactory`, `CameraGpu`, fit-to-AABB, screen ray, camera–transform sync.
- **Lights**: `ILight`, `AmbientLight`, `DirectionalLight`, `PointLight`, `SpotLight`, `LightGpu`, shadow settings.
- **Scene state**: `SceneState` (active camera + lights, max lights policy).
- **Environment**: Exposure, gamma, clear color, optional HDRI asset id.
- **Cross-platform**: Linux, macOS, Windows (and optionally iOS, Android, Web via vnemath).

## Installation

### Option 1: Git Submodule (Recommended)

```bash
git submodule add https://github.com/vertexnova/vnescene.git external/vnescene
# Ensure vnemath (and optionally vnelogging) are available as dependencies.
```

In your `CMakeLists.txt`:

```cmake
add_subdirectory(external/vnescene)
target_link_libraries(your_target PRIVATE vne::scene)
```

### Option 2: FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    vnescene
    GIT_REPOSITORY https://github.com/vertexnova/vnescene.git
    GIT_TAG main
)
FetchContent_MakeAvailable(vnescene)
target_link_libraries(your_target PRIVATE vne::scene)
```

### Option 3: System Install

```bash
git clone --recursive https://github.com/vertexnova/vnescene.git
cd vnescene
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

In your `CMakeLists.txt` (ensure [vnemath](https://github.com/vertexnova/vnemath) is installed first):

```cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_PREFIX_PATH}/lib/cmake/VneScene")
find_package(VneScene REQUIRED)
target_link_libraries(your_target PRIVATE vne::scene)
```

Configure with `-DCMAKE_PREFIX_PATH=/usr/local` (or your install prefix) so the Find module is discovered.

## Building

```bash
git clone --recursive https://github.com/vertexnova/vnescene.git
cd vnescene
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

For local development (examples + tests enabled):

```bash
cmake -B build -DVNE_SCENE_DEV=ON
cmake --build build
```

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `VNE_SCENE_TESTS` | `ON` | Build the test suite |
| `VNE_SCENE_EXAMPLES` | `OFF` | Build example applications |
| `VNE_SCENE_DEV` | `ON` (top-level) | Dev preset: tests and examples ON |
| `VNE_SCENE_CI` | `OFF` | CI preset: tests ON, examples OFF |
| `ENABLE_DOXYGEN` | `OFF` | Build API documentation (Doxygen) |
| `ENABLE_COVERAGE` | `OFF` | Enable code coverage reporting |

## Quick Start

```cpp
#include <vertexnova/scene/scene.h>
#include <memory>

int main() {
    using namespace vne::scene;
    using namespace vne::math;

    auto cam = CameraFactory::createPerspective(
        PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 100.0f));
    cam->setPosition(Vec3f(0.0f, 2.0f, 5.0f));

    SceneState state;
    state.setActiveCamera(cam);
    state.addLight(std::make_shared<AmbientLight>(Vec3f(0.2f, 0.2f, 0.2f), 1.0f));

    if (state.hasActiveCamera()) {
        CameraGpu cam_gpu = state.getActiveCamera()->toGpu();
        // Upload cam_gpu to GPU or pass to renderer
    }
    return 0;
}
```

See [examples/01_basic](examples/01_basic) for a complete minimal example.

## Examples

| Example | Description |
|---------|-------------|
| [01_basic](examples/01_basic) | Minimal scene: camera and lights with logging |
| [02_scene_gpu_pack](examples/02_scene_gpu_pack) | SceneState and packing camera/lights for GPU |
| [03_camera_backend](examples/03_camera_backend) | setGraphicsApi (OpenGL/Vulkan), matrix logging |
| [04_fit_to_aabb](examples/04_fit_to_aabb) | fitToAabb and project AABB corners |
| [05_screen_ray](examples/05_screen_ray) | screenToWorldRay at center pixel |
| [06_shadow_settings](examples/06_shadow_settings) | Directional and spot lights, setShadowSettings |
| [07_camera_transform_sync](examples/07_camera_transform_sync) | syncCameraFromTransformNode / syncTransformNodeFromCamera |

Build with `-DVNE_SCENE_EXAMPLES=ON` or use the dev preset (`-DVNE_SCENE_DEV=ON`). Run from `build/bin/examples/`. See [examples/README.md](examples/README.md) for details.

## Documentation

- [API Documentation](docs/README.md) — Generate with Doxygen (`-DENABLE_DOXYGEN=ON`, then `cmake --build build --target vnescene_doc_doxygen`).
- [Architecture & design](docs/vertexnova/scene/scene.md) — Module overview, components, and usage.

## Platform Support

| Platform | Status | Notes |
|----------|--------|-------|
| Linux | Supported | GCC 9+, Clang 10+ |
| macOS | Supported | Xcode 12+, Apple Clang |
| Windows | Supported | MSVC 2019+, MinGW |
| iOS / visionOS | Supported | Via vnemath toolchain |
| Android / Web | Supported | Via vnemath |

## Requirements

- C++20
- CMake 3.19+
- [vnemath](https://github.com/vertexnova/vnemath) (required)
- vnelogging (optional; for examples)

## License

Apache License 2.0 — see [LICENSE](LICENSE) for details.

---

Part of the [VertexNova](https://github.com/vertexnova) project.
