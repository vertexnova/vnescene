# Scene Module

## Overview

The Scene module provides cameras, lights, and scene state building blocks for VertexNova. It does not implement ECS, rendering, or input; it focuses on view (cameras) and lighting data with GPU-friendly packing for use by a renderer or viewer.

**Key characteristics:**

- **Cameras**: Perspective and orthographic cameras with view/projection matrices, GPU pack layout, fit-to-AABB, screen-to-world ray, and transform sync with external nodes.
- **Lights**: Ambient, directional, point, and spot lights with a common GPU layout and optional shadow settings.
- **SceneState**: Single active camera plus a list of lights (with optional max count); suitable for sandbox and viewer use cases.
- **Handles**: Type-safe generational handles (e.g. `CameraId`, `LightId`) for identity without raw pointers.

## Architecture

The module is organized in layers:

- **Public API** (`include/vertexnova/scene/`): Headers under `camera/`, `light/`, plus `handle.h`, `scene_state.h`, `environment.h`, and the umbrella `scene.h`.
- **Implementation** (`src/`): Camera and light implementations, scene state, and GPU packing logic.
- **Dependency**: Uses **vnemath** for matrices, vectors, rays, and math types. Examples optionally use **vnelogging** for console output.

There is no dependency on a specific graphics API; camera and light data are packed into structs (e.g. `CameraGpu`, `LightGpu`) that the application can upload to the GPU.

## Key Components

### Handles

- **handle.h**: `Handle<Tag>` — generational handle (32-bit slot + 32-bit generation). `CameraId` and `LightId` are distinct types to avoid misuse.

### Cameras

- **camera.h**: `ICamera` — base interface (view/projection, position/target/up, name, active flag).
- **camera_types.h**: `CameraType`, `CameraParameters`, and related types.
- **perspective_camera.h**: `PerspectiveCamera` — perspective projection, FOV, aspect, near/far.
- **orthographic_camera.h**: `OrthographicCamera` — ortho projection and bounds.
- **camera_factory.h**: `CameraFactory` — create cameras by type.
- **camera_utils.h**: Utilities such as `fitToAabb`, `projectAabbCorners`, `screenToWorldRay`.
- **camera_gpu.h**: `CameraGpu` — packed layout for GPU (view/projection, position, etc.).
- **camera_transform_adapter.h**: `CameraTransformAdapter` — sync camera from a transform node or sync a transform node from the camera.

### Lights

- **light.h**: `ILight` interface and `LightGpu` — common GPU layout; light types (ambient, directional, point, spot).
- **light_gpu.h**: GPU layout details.
- **ambient_light.h**, **directional_light.h**, **point_light.h**, **spot_light.h**: Concrete light implementations with shadow settings where applicable.

### Scene State and Environment

- **scene_state.h**: `SceneState` — `setActiveCamera` / `getActiveCamera`, `addLight` / `removeLight` / `getLights`, `setMaxLights` / `getMaxLights`.
- **environment.h**: `Environment` — exposure, gamma, clear color, optional HDRI asset id (data only; no GPU textures).

### Umbrella Header

- **scene.h**: Includes the full public API and provides `packLightGpu(ILight)` for convenience.

## Usage Examples

### Minimal: camera and scene state

```cpp
#include <vertexnova/scene/scene.h>

int main() {
    auto camera = std::make_shared<vne::scene::PerspectiveCamera>(
        /* position */ vne::math::Vec3f{0, 0, 5},
        /* target   */ vne::math::Vec3f{0, 0, 0},
        /* up      */ vne::math::Vec3f{0, 1, 0},
        /* fovDeg  */ 60.0f,
        /* aspect  */ 16.0f / 9.0f,
        /* near    */ 0.1f,
        /* far     */ 100.0f
    );

    vne::scene::SceneState state;
    state.setActiveCamera(camera);
    state.addLight(std::make_shared<vne::scene::AmbientLight>(vne::math::Vec3f{0.2f}));

    // Pack for GPU or pass to renderer
    if (state.hasActiveCamera()) {
        vne::scene::CameraGpu cam_gpu = state.getActiveCamera()->toGpu();
        // ... upload cam_gpu to GPU
    }
    return 0;
}
```

### Fit camera to AABB and screen ray

See **examples 04_fit_to_aabb** and **05_screen_ray** for `fitToAabb`, `projectAabbCorners`, and `screenToWorldRay`.

### Camera transform sync

See **example 07_camera_transform_sync** for `syncCameraFromTransformNode` and `syncTransformNodeFromCamera`.

## Dependencies

- **vnemath**: Matrices, vectors, rays, and core math types. Required.
- **vnelogging**: Optional; used by the example programs for console output.

## See Also

- [Documentation index](../../README.md) — how to generate API documentation (Doxygen).
- [Examples](../../../examples/README.md) — 01_basic through 07_camera_transform_sync.
