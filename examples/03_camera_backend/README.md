# 03 — Camera graphics backend

## What it covers

- `setGraphicsApi` **OpenGL** vs **Vulkan**
- **Projection matrix** Y scale (`proj[1][1]`) differs in sign between APIs (NDC convention)

## Purpose

Regression guard for backend-specific projection. The example asserts OpenGL and Vulkan Y scales have **opposite signs**.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_03_camera_backend`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_03_camera_backend
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_03_camera_backend`; with **`cmake -B build`** run `./build/bin/examples/example_03_camera_backend`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
