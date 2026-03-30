#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   March 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

/**
 * @file camera_base.h
 * @brief Shared state and logic for concrete camera implementations.
 *
 * This header is part of the public scene API surface because
 * PerspectiveCamera and OrthographicCamera (both public types) inherit
 * from CameraBase. However, CameraBase itself is not a public polymorphic
 * interface like ICamera — it carries only data and shared implementations
 * for position/target/up, dirty-flag management, scene-scale, name, and
 * active state. All constructors and members are protected.
 */

#include "vertexnova/scene/export.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class CameraBase
 * @brief Protected non-virtual base for PerspectiveCamera and OrthographicCamera.
 *
 * Holds all shared member variables and provides default implementations
 * for methods that are identical across both camera types. Concrete cameras
 * inherit this class using @c protected inheritance and call its helpers
 * from their ICamera overrides.
 */
class VNE_SCENE_API CameraBase {
   protected:
    CameraBase() = default;
    explicit CameraBase(std::string name) noexcept
        : name_(std::move(name)) {}
    ~CameraBase() noexcept = default;

    // Copy/move defaulted; concrete cameras manage their own copy/move semantics.
    CameraBase(const CameraBase&) = default;
    CameraBase& operator=(const CameraBase&) = default;
    CameraBase(CameraBase&&) noexcept = default;
    CameraBase& operator=(CameraBase&&) noexcept = default;

    //--------------------------------------------------------------------------
    // Pose helpers — set position/target/up atomically with one dirty mark
    //--------------------------------------------------------------------------

    /** @brief Set position, target, and up at once; marks view dirty. */
    void lookAtImpl(const vne::math::Vec3f& position,
                    const vne::math::Vec3f& target,
                    const vne::math::Vec3f& up) noexcept {
        position_ = position;
        target_ = target;
        up_ = up;
        view_matrix_dirty_ = true;
    }

    /** @brief Keep current position; change target and up; marks view dirty. */
    void lookAtImpl(const vne::math::Vec3f& target, const vne::math::Vec3f& up) noexcept {
        target_ = target;
        up_ = up;
        view_matrix_dirty_ = true;
    }

    /**
     * @brief Apply scene zoom in the image plane after lookAt.
     *
     * Uses scale(s, s, 1) so view-space depth is not scaled; projection near/far stay consistent
     * with geometry (uniform scale(s,s,s) on the view broke clip-space depth).
     *
     * @param look_at_view View matrix from Mat4f::lookAt (no scene scale applied yet).
     * @param scene_scale XY scale factor; 1.0 returns @a look_at_view unchanged.
     */
    [[nodiscard]] static vne::math::Mat4f composeViewWithSceneScale(const vne::math::Mat4f& look_at_view,
                                                                    float scene_scale) noexcept {
        if (scene_scale == 1.0f) {
            return look_at_view;
        }
        return vne::math::Mat4f::scale(scene_scale, scene_scale, 1.0f) * look_at_view;
    }

    //--------------------------------------------------------------------------
    // Shared member state
    //--------------------------------------------------------------------------

    vne::math::Vec3f position_{0.0f, 0.0f, 0.0f};  //!< Camera position in world space.
    vne::math::Vec3f target_{0.0f, 0.0f, -1.0f};   //!< Look-at target point.
    vne::math::Vec3f up_{0.0f, 1.0f, 0.0f};        //!< Up vector for view orientation.
    std::string name_;                             //!< Camera name.
    bool active_ = true;                           //!< Whether this camera is active for rendering.
    vne::math::GraphicsApi graphics_api_{vne::math::GraphicsApi::eOpenGL};  //!< Backend for view/projection.
    float scene_scale_ = 1.0f;  //!< XY zoom factor baked into the view (see composeViewWithSceneScale).

    mutable vne::math::Mat4f view_matrix_{vne::math::Mat4f::identity()};             //!< Cached view matrix.
    mutable vne::math::Mat4f projection_matrix_{vne::math::Mat4f::identity()};       //!< Cached projection matrix.
    mutable vne::math::Mat4f view_projection_matrix_{vne::math::Mat4f::identity()};  //!< Cached view * projection.
    mutable bool view_matrix_dirty_ = true;        //!< True when view matrix needs recomputation.
    mutable bool projection_matrix_dirty_ = true;  //!< True when projection matrix needs recomputation.
};

}  // namespace vne::scene
