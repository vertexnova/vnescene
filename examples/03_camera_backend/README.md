# 03 — Camera graphics backend

## What it covers

- `setGraphicsApi` **OpenGL** vs **Vulkan**
- **Projection matrix** Y scale (`proj[1][1]`) differs in sign between APIs (NDC convention)

## Purpose

Regression guard for backend-specific projection. The example asserts OpenGL and Vulkan Y scales have **opposite signs**.

## Build and run

```bash
./build/bin/examples/example_03_camera_backend
```
