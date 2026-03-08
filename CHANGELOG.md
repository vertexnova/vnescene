# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.1.0](https://github.com/vertexnova/vnescene/compare/v1.0.0...v1.1.0) (2026-03-08)


### Features

* Add unit tests for camera, environment, handle, light, and scen… ([5da33d5](https://github.com/vertexnova/vnescene/commit/5da33d58493ac5d8c6e496f36b50da67a5255ef6))
* Add unit tests for camera, environment, handle, light, and scene state functionalities ([cc7d55d](https://github.com/vertexnova/vnescene/commit/cc7d55d453902fe9490bb751b76d7f48d747b27f))
* add VNE_SCENE_API export macro to Environment, ShadowCascadeSettings, and ShadowSettings structs for improved DLL compatibility ([1d37242](https://github.com/vertexnova/vnescene/commit/1d3724251101d52dd332c6ea6ca4567ce4b0e89f))
* Merge pull request [#1](https://github.com/vertexnova/vnescene/issues/1) from vertexnova/VNE_SCENE_INITIAL_SETUP ([5da33d5](https://github.com/vertexnova/vnescene/commit/5da33d58493ac5d8c6e496f36b50da67a5255ef6))


### Bug Fixes

* ensure .gcno and .gcda file listings do not fail the build ([bc447b4](https://github.com/vertexnova/vnescene/commit/bc447b40d2d72c9b7ce95c3542cb0c85d83f12fc))
* qualify updateMatrices calls in OrthographicCamera and PerspectiveCamera constructors ([9166917](https://github.com/vertexnova/vnescene/commit/916691705cb1045e4515697ac3e8b79cb11a3bcd))
* register vnemath as a proper git submodule ([22930c1](https://github.com/vertexnova/vnescene/commit/22930c1a108032063b01b45ddeeab1c057e6b61c))
* suppress MSVC warning C4251 for non-exported types in exported class members ([67b32fb](https://github.com/vertexnova/vnescene/commit/67b32fb5550c6bf797492a92b735063d651d61cf))

## [1.0.0] - 2026-03-08

### Added

- Initial release of VneScene: scene library with cameras (ICamera, PerspectiveCamera, OrthographicCamera, factory, utils, GPU, transform adapter), lights (ILight, Ambient, Directional, Point, Spot, LightGpu), SceneState, Handle, Environment, gpu_types. Supports static and shared builds with DLL import/export on Windows.
