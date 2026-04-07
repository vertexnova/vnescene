# 05 — Screen ray

## What it covers

- `screenToWorldRay` at viewport center
- Ray **direction** unit length
- Ray **origin** matches **camera position**

## Purpose

Sanity-check picking / raycast math against the current camera matrices.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_05_screen_ray`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_05_screen_ray
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_05_screen_ray`; with **`cmake -B build`** run `./build/bin/examples/example_05_screen_ray`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
