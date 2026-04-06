# 01 — Basic scene

## What it covers

- **Perspective camera** via `CameraFactory::createPerspective`, `lookAt`, `updateMatrices`
- **Quaternion pose checks**: `getForwardDir()` unit length, `getOrientation()` normalized, forward aligned with derived `(getTarget() - getPosition())`
- **Orthographic camera** via `CameraFactory::createOrthographic` and `getForwardDir()`
- **Lights**: `AmbientLight`, `DirectionalLight` construction and names

## Purpose

Smoke-test linkage and the quaternion-native camera API. A non-zero exit means one of the self-checks failed (see `[FAIL]` in the log).

## Build and run

From the vnescene root (with examples enabled):

```bash
cmake -B build -DVNE_SCENE_EXAMPLES=ON
cmake --build build
./build/bin/examples/example_01_basic
echo $?   # expect 0
```

See [../README.md](../README.md) for shared options (`VNE_SCENE_DEV`, static vs shared).
