# 06 — Shadow settings

## What it covers

- `ShadowSettings` on **DirectionalLight** (enabled, resolution, bias, far_plane)
- `ShadowSettings` on **SpotLight** (enabled, resolution)
- Read-back via `getShadowSettings()` matches what was set

## Purpose

Quick API smoke for shadow parameters stored on lights.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_06_shadow_settings`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_06_shadow_settings
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_06_shadow_settings`; with **`cmake -B build`** run `./build/bin/examples/example_06_shadow_settings`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
