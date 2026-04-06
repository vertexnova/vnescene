# 02 — Scene GPU pack

## What it covers

- **`SceneState`**: active camera, `addLight`, light count
- **`CameraGpu`**: `toGpu()` near/far/viewport fields
- **Direction vectors**: packed `forward_dir`, `right_dir`, `up_dir` must match `getForwardDir()` / `getRightDir()` / `getUpDir()`
- **`LightGpu`**: `packLightGpu` for each light

## Purpose

Validates GPU packing stays consistent with the camera’s quaternion basis. Non-zero exit indicates a mismatch between CPU and packed directions.

## Build and run

```bash
./build/bin/examples/example_02_scene_gpu_pack
```
