# VneScene Examples

This directory contains headless examples that exercise the VneScene API. Each numbered folder has its **own README** with details, expected behavior, and run commands.

**Exit codes:** `0` means all built-in self-checks passed. A **non-zero** exit means at least one check failed (look for `[FAIL]` in the log). This supports CI or scripted smoke runs without linking GoogleTest into examples.

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

Executables are placed in `build/<config>/bin/examples/`.

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

**Run (replace `shared` with your build dir):**

```bash
./build/shared/bin/examples/example_01_basic
./build/shared/bin/examples/example_02_scene_gpu_pack
# … etc.
```

## CMake layout

[`CMakeLists.txt`](CMakeLists.txt) defines `vne_add_example(target …)` so each subdirectory only registers sources; linking and include paths stay consistent.
