# 04 — Fit to AABB

## What it covers

- `fitToAabb` on a `PerspectiveCamera`
- `project` for all **8** AABB corners (screen x, y, OpenGL depth z)
- Self-check: every corner has finite coordinates and **depth in [0, 1]**

## Purpose

Ensures framing + projection keep the entire box inside the clip volume for the chosen API.

## Build and run

```bash
./build/bin/examples/example_04_fit_to_aabb
```
