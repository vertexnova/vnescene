# 07 — Camera / transform sync

## What it covers

- `syncTransformNodeFromCamera`: node world translation matches camera **position**
- `syncCameraFromTransformNode`: after `setLocalTransform(translate(...))`, camera **position** matches that translation

## Purpose

Validates the transform adapter against a simple translate-only node path.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_07_camera_transform_sync`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_07_camera_transform_sync
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_07_camera_transform_sync`; with **`cmake -B build`** run `./build/bin/examples/example_07_camera_transform_sync`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
