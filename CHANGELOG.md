# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-03-08

### Added

- Initial release of VneScene: scene library with cameras (ICamera, PerspectiveCamera, OrthographicCamera, factory, utils, GPU, transform adapter), lights (ILight, Ambient, Directional, Point, Spot, LightGpu), SceneState, Handle, Environment, gpu_types. Supports static and shared builds with DLL import/export on Windows.
