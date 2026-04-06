# VneScene Examples

This directory contains headless examples that exercise the VneScene API. Each numbered folder has its **own README** with details, expected behavior, and run commands.

**Exit codes:** `0` means all built-in self-checks passed. A **non-zero** exit means at least one check failed (look for `[FAIL]` in the log). This supports CI or scripted smoke runs without linking GoogleTest into examples.

**CTest:** When `VNE_SCENE_EXAMPLES=ON`, each example is registered as a test (same name as the executable, e.g. `example_01_basic`). Run `ctest --output-on-failure` from the build tree to execute them together with `vnescene_tests` (if `VNE_SCENE_TESTS=ON`). Filter examples only: `ctest -L examples`.

Shared helpers live under [`common/`](common/): [`logging_guard.h`](common/logging_guard.h) and [`example_utils.h`](common/example_utils.h) (section banners, `checkNear` / `checkVec3Near` / `checkLengthNear` / `checkTrue`).

## Building Examples

From the project root:

```bash
# Shared library build
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared

# Static library build
cmake -B build/static -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=static
cmake --build build/static
```

Alternatively, `-DVNE_SCENE_DEV=ON` enables both tests and examples.

Executables are placed in **`${CMAKE_BINARY_DIR}/bin/examples/`** (set in [CMakeLists.txt](CMakeLists.txt) via `RUNTIME_OUTPUT_DIRECTORY`). That is your CMake binary directory (the path passed to **`cmake -B`**) with **`/bin/examples/`** appended—e.g. `build/shared/bin/examples/` or `build/static/bin/examples/` for the layouts above, or `build/bin/examples/` if you use `cmake -B build`.

## Index

| Example | Focus | README |
|---------|--------|--------|
| 01_basic | Cameras, quaternion pose, lights | [01_basic/README.md](01_basic/README.md) |
| 02_scene_gpu_pack | SceneState, CameraGpu, LightGpu | [02_scene_gpu_pack/README.md](02_scene_gpu_pack/README.md) |
| 03_camera_backend | OpenGL vs Vulkan projection | [03_camera_backend/README.md](03_camera_backend/README.md) |
| 04_fit_to_aabb | fitToAabb, project | [04_fit_to_aabb/README.md](04_fit_to_aabb/README.md) |
| 05_screen_ray | screenToWorldRay | [05_screen_ray/README.md](05_screen_ray/README.md) |
| 06_shadow_settings | Shadow settings on lights | [06_shadow_settings/README.md](06_shadow_settings/README.md) |
| 07_camera_transform_sync | Camera ↔ TransformNode | [07_camera_transform_sync/README.md](07_camera_transform_sync/README.md) |

**Run** (after the same configure/build as above; binaries live under **`${CMAKE_BINARY_DIR}/bin/examples/`**):

```bash
./build/shared/bin/examples/example_01_basic
./build/shared/bin/examples/example_02_scene_gpu_pack
./build/shared/bin/examples/example_03_camera_backend
./build/shared/bin/examples/example_04_fit_to_aabb
./build/shared/bin/examples/example_05_screen_ray
./build/shared/bin/examples/example_06_shadow_settings
./build/shared/bin/examples/example_07_camera_transform_sync
```

With **`cmake -B build/static`** or **`cmake -B build`**, replace the `./build/shared` prefix with `./build/static` or `./build` respectively. Each example’s README in this directory uses the same **Build and run** layout for its executable name.

## CMake layout

[`CMakeLists.txt`](CMakeLists.txt) defines `vne_add_example(target …)` so each subdirectory only registers sources; linking and include paths stay consistent.
