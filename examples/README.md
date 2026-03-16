# VneScene Examples

This directory contains examples demonstrating the VneScene API.

## Building Examples

From the project root (use `build/shared` or `build/static`):

```bash
# Shared library build
cmake -B build/shared -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=shared
cmake --build build/shared

# Static library build
cmake -B build/static -DVNE_SCENE_EXAMPLES=ON -DVNE_SCENE_LIB_TYPE=static
cmake --build build/static
```

Alternatively, `-DVNE_SCENE_DEV=ON` enables both tests and examples.

Executables are placed in `build/shared/bin/examples/` (or `build/static/bin/examples/`).

## Available Examples

### 01_basic — Getting Started

Minimal scene: create a perspective camera via `CameraFactory`, set position, and create ambient/directional lights.

### 02_scene_gpu_pack

Packs scene state (active camera, lights) into GPU-friendly structures (`CameraGpu`, `LightGpu`).

### 03_camera_backend

Switches camera graphics API (OpenGL vs Vulkan) and shows how projection matrix Y convention changes.

### 04_fit_to_aabb

Fits a perspective camera to an AABB using `fitToAabb`, then projects AABB corners to screen with `project`.

### 05_screen_ray

Computes a world-space ray from screen coordinates using `screenToWorldRay`.

### 06_shadow_settings

Configures shadow settings on directional and spot lights.

### 07_camera_transform_sync

Syncs camera pose with a transform node: `syncTransformNodeFromCamera` and `syncCameraFromTransformNode`.

**Run:** `./build/<lib_type>/bin/examples/example_01_basic` (and similarly for `example_02_scene_gpu_pack`, etc.). Use `shared` or `static` to match your build.

## Quick Reference

| Example                 | Focus              | Key Concepts                                      |
|-------------------------|--------------------|---------------------------------------------------|
| 01_basic                | Getting started    | CameraFactory, camera pose, lights                |
| 02_scene_gpu_pack       | GPU packing        | SceneState, CameraGpu, LightGpu                   |
| 03_camera_backend       | Graphics API       | setGraphicsApi, projection matrix                |
| 04_fit_to_aabb          | Camera framing     | fitToAabb, project                                |
| 05_screen_ray           | Picking            | screenToWorldRay                                  |
| 06_shadow_settings      | Shadows            | shadow settings on lights                        |
| 07_camera_transform_sync| Camera–node sync   | syncTransformNodeFromCamera, syncCameraFromTransformNode |
