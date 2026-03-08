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
 * @file camera_types.h
 * @brief Shared enums and structs for cameras (no input/manipulator types).
 */

#include "vertexnova/scene/export.h"
#include <vertexnova/math/core/core.h>
#include <cstdint>
#include <string>

namespace vne::scene {

/**
 * @enum CameraType
 * @brief Specifies the kind of camera projection.
 */
enum class CameraType : std::uint8_t {
    ePerspective = 0,   //!< Perspective (FOV-based) projection.
    eOrthographic = 1,  //!< Orthographic (parallel) projection.
};

/**
 * @struct CameraParameters
 * @brief Base parameters common to all cameras.
 *
 * Virtual dtor allows polymorphic use in factory. Derived structs add
 * type-specific fields (e.g. FOV, aspect, ortho bounds).
 */
struct VNE_SCENE_API CameraParameters {
    CameraType type = CameraType::ePerspective;    //!< Camera projection type.
    std::string name;                              //!< Camera name.
    vne::math::Vec3f position{0.0f, 0.0f, 10.0f};  //!< World-space position.
    vne::math::Vec3f target{0.0f, 0.0f, 0.0f};     //!< Look-at target.
    vne::math::Vec3f up{0.0f, 1.0f, 0.0f};         //!< Up vector.
    float near_plane = 0.1f;                       //!< Near clip plane.
    float far_plane = 1000.0f;                     //!< Far clip plane.

    CameraParameters() = default;
    explicit CameraParameters(CameraType camera_type)
        : type(camera_type) {}
    virtual ~CameraParameters() noexcept = default;
};

/**
 * @struct PerspectiveCameraParameters
 * @brief Parameters for creating a perspective camera.
 */
struct VNE_SCENE_API PerspectiveCameraParameters : CameraParameters {
    float fov = 45.0f;
    float aspect_ratio = 16.0f / 9.0f;
    float viewport_width = 0.0f;
    float viewport_height = 0.0f;

    PerspectiveCameraParameters()
        : CameraParameters(CameraType::ePerspective) {}
    PerspectiveCameraParameters(float field_of_view, float aspect, float near_clip, float far_clip)
        : CameraParameters(CameraType::ePerspective)
        , fov(field_of_view)
        , aspect_ratio(aspect) {
        near_plane = near_clip;
        far_plane = far_clip;
    }
    PerspectiveCameraParameters(float field_of_view, float width, float height, float near_clip, float far_clip)
        : CameraParameters(CameraType::ePerspective)
        , fov(field_of_view)
        , viewport_width(width)
        , viewport_height(height) {
        near_plane = near_clip;
        far_plane = far_clip;
        if (height > 0.0f) {
            aspect_ratio = width / height;
        }
    }
};

/**
 * @struct OrthographicCameraParameters
 * @brief Parameters for creating an orthographic camera.
 */
struct VNE_SCENE_API OrthographicCameraParameters : CameraParameters {
    float left = -10.0f;
    float right = 10.0f;
    float bottom = -10.0f;
    float top = 10.0f;
    float viewport_width = 0.0f;
    float viewport_height = 0.0f;

    OrthographicCameraParameters()
        : CameraParameters(CameraType::eOrthographic) {}
    OrthographicCameraParameters(float l, float r, float b, float t, float near_clip, float far_clip)
        : CameraParameters(CameraType::eOrthographic)
        , left(l)
        , right(r)
        , bottom(b)
        , top(t) {
        near_plane = near_clip;
        far_plane = far_clip;
    }
    OrthographicCameraParameters(float width, float height, float scale, float near_clip, float far_clip)
        : CameraParameters(CameraType::eOrthographic)
        , viewport_width(width)
        , viewport_height(height) {
        near_plane = near_clip;
        far_plane = far_clip;
        float half_width = (width * 0.5f) * scale;
        float half_height = (height * 0.5f) * scale;
        left = -half_width;
        right = half_width;
        bottom = -half_height;
        top = half_height;
    }
};

}  // namespace vne::scene
