# 02 — Scene GPU pack

## What it covers

- **`SceneState`**: active camera, `addLight`, light count
- **`CameraGpu`**: `toGpu()` near/far/viewport fields
- **Direction vectors**: packed `forward_dir`, `right_dir`, `up_dir` must match `getForwardDir()` / `getRightDir()` / `getUpDir()`
- **`LightGpu`**: `packLightGpu` for each light

## Purpose

Validates GPU packing stays consistent with the camera’s quaternion basis. Non-zero exit indicates a mismatch between CPU and packed directions.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_02_scene_gpu_pack`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_02_scene_gpu_pack
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_02_scene_gpu_pack`; with **`cmake -B build`** run `./build/bin/examples/example_02_scene_gpu_pack`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
