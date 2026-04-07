# 04 — Fit to AABB

## What it covers

- `fitToAabb` on a `PerspectiveCamera`
- `project` for all **8** AABB corners (screen x, y, OpenGL depth z)
- Self-check: every corner has finite coordinates and **depth in [0, 1]**

## Purpose

Ensures framing + projection keep the entire box inside the clip volume for the chosen API.

## Build and run

From the vnescene repository root. The executable is **`${CMAKE_BINARY_DIR}/bin/examples/example_04_fit_to_aabb`** (CMake `RUNTIME_OUTPUT_DIRECTORY` in [examples/CMakeLists.txt](../CMakeLists.txt); overview in [examples/README.md](../README.md)).

**Recommended** (shared library build tree):

```bash
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared
./build/shared/bin/examples/example_04_fit_to_aabb
echo $?   # expect 0
```

**Other binary directories:** with **`cmake -B build/static`** run `./build/static/bin/examples/example_04_fit_to_aabb`; with **`cmake -B build`** run `./build/bin/examples/example_04_fit_to_aabb`.

See the repository [README.md](../README.md) for `VNE_SCENE_DEV` and further options.
