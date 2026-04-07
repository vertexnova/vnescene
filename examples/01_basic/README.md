# 01 — Basic scene

## What it covers

- **Perspective camera** via `CameraFactory::createPerspective`, `lookAt`, `updateMatrices`
- **Quaternion pose checks**: `getForwardDir()` unit length, `getOrientation()` normalized, forward aligned with derived `(getTarget() - getPosition())`
- **Orthographic camera** via `CameraFactory::createOrthographic` and `getForwardDir()`
- **Lights**: `AmbientLight`, `DirectionalLight` construction and names

## Purpose

Smoke-test linkage and the quaternion-native camera API. A non-zero exit means one of the self-checks failed (see `[FAIL]` in the log).

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_01_basic`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_01_basic
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_01_basic`; with **`cmake -B build`** run `./build/bin/examples/example_01_basic`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
