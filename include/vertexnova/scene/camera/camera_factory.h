#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   February 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

/**
 * @file camera_factory.h
 * @brief Factory for creating cameras from parameter structs.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/camera_types.h"
#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/orthographic_camera.h"
#include <memory>

namespace vne::scene {

/**
 * @class CameraFactory
 * @brief Factory for creating cameras from parameters.
 *
 * Static methods only; use create() for polymorphic creation from
 * CameraParameters, or createPerspective / createOrthographic for
 * concrete types.
 */
class VNE_SCENE_API CameraFactory {
   public:
    CameraFactory() = delete;

    /**
     * @brief Create a perspective camera from the given parameters.
     * @param params Perspective parameters (FOV, aspect, near/far, etc.).
     * @return Shared pointer to a new PerspectiveCamera.
     */
    [[nodiscard]] static std::shared_ptr<PerspectiveCamera> createPerspective(
        const PerspectiveCameraParameters& params) noexcept;

    /**
     * @brief Create an orthographic camera from the given parameters.
     * @param params Orthographic parameters (left, right, bottom, top, near, far).
     * @return Shared pointer to a new OrthographicCamera.
     */
    [[nodiscard]] static std::shared_ptr<OrthographicCamera> createOrthographic(
        const OrthographicCameraParameters& params) noexcept;

    /**
     * @brief Create a camera from generic parameters (type taken from params.type).
     * @param params Camera parameters (Perspective or Orthographic).
     * @return Shared pointer to ICamera (PerspectiveCamera or OrthographicCamera).
     */
    [[nodiscard]] static std::shared_ptr<ICamera> create(const CameraParameters& params) noexcept;
};

}  // namespace vne::scene
