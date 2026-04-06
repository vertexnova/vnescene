# 07 — Camera / transform sync

## What it covers

- `syncTransformNodeFromCamera`: node world translation matches camera **position**
- `syncCameraFromTransformNode`: after `setLocalTransform(translate(...))`, camera **position** matches that translation

## Purpose

Validates the transform adapter against a simple translate-only node path.

## Build and run

```bash
./build/bin/examples/example_07_camera_transform_sync
```
