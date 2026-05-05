# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.5.1](https://github.com/vertexnova/vnescene/compare/v1.5.0...v1.5.1) (2026-05-05)


### Bug Fixes

* Simplify near plane handling in OrthographicCamera ([#21](https://github.com/vertexnova/vnescene/issues/21)) ([17ad896](https://github.com/vertexnova/vnescene/commit/17ad896adebf6c07bd40fa079920b8a4c35e393d))

## [1.5.0](https://github.com/vertexnova/vnescene/compare/v1.4.1...v1.5.0) (2026-05-05)


### Features

* Add standalone installation option for vnescene ([#18](https://github.com/vertexnova/vnescene/issues/18)) ([f26c755](https://github.com/vertexnova/vnescene/commit/f26c755cc6383975de284794064ed3957f9fb99c))

## [1.4.1](https://github.com/vertexnova/vnescene/compare/v1.4.0...v1.4.1) (2026-05-05)


### Bug Fixes

* Fixing vne scene bug fixes ([#16](https://github.com/vertexnova/vnescene/issues/16)) ([8c3c1ab](https://github.com/vertexnova/vnescene/commit/8c3c1ab2fe4a2ecf2bd1285a00aa5f3b3c9b24bc))

## [1.4.0](https://github.com/vertexnova/vnescene/compare/v1.3.2...v1.4.0) (2026-04-07)


### Features

* Enhanced camera system with quaternion based pose rotation. Updated camera documention, texts and examples. ([#14](https://github.com/vertexnova/vnescene/issues/14)) ([dc8591d](https://github.com/vertexnova/vnescene/commit/dc8591dc80c445d10ab55ba2bac13720e91254f1))

## [1.3.2](https://github.com/vertexnova/vnescene/compare/v1.3.1...v1.3.2) (2026-03-30)


### Bug Fixes

* fixing scene scaling functionality in camera system ([#12](https://github.com/vertexnova/vnescene/issues/12)) ([4363eb5](https://github.com/vertexnova/vnescene/commit/4363eb5687a6f26daeb9799059ba7a7cbdb591a8))

## [1.3.1](https://github.com/vertexnova/vnescene/compare/v1.3.0...v1.3.1) (2026-03-25)


### Bug Fixes

* Suppress MSVC warnings for exported camera types in camera_types.h ([#10](https://github.com/vertexnova/vnescene/issues/10)) ([e35d09a](https://github.com/vertexnova/vnescene/commit/e35d09ab1c8d5f4212d58cf4f023cfb57dc95020))

## [1.3.0](https://github.com/vertexnova/vnescene/compare/v1.2.0...v1.3.0) (2026-03-23)


### Features

* Enhance camera system with projection type retrieval and utilit… ([#8](https://github.com/vertexnova/vnescene/issues/8)) ([f4f91e1](https://github.com/vertexnova/vnescene/commit/f4f91e1488f8a95c0602016e7dd2b2d7e18396af))

## [1.2.0](https://github.com/vertexnova/vnescene/compare/v1.1.1...v1.2.0) (2026-03-16)


### Features

* Adding more feature, refactoring and api changes for vnescene camera. ([#6](https://github.com/vertexnova/vnescene/issues/6)) ([3db86ab](https://github.com/vertexnova/vnescene/commit/3db86abbe846ef004a3708778c8afb5034cbe2bc))

## [1.1.1](https://github.com/vertexnova/vnescene/compare/v1.1.0...v1.1.1) (2026-03-10)


### Bug Fixes

* fixing ci issue for failing for ios ([#4](https://github.com/vertexnova/vnescene/issues/4)) ([31cfb2d](https://github.com/vertexnova/vnescene/commit/31cfb2dc53443674042bddc51bbae9d3282e7f3b))

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
