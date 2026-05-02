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
 * interface like ICamera — it carries data and shared implementations
 * for quaternion-native pose, dirty-flag management, scene-scale, name, and
 * active state. All constructors and members are protected.
 */

#include "vertexnova/scene/export.h"
#include <vertexnova/math/core/core.h>
#include <string>
#include <utility>

namespace vne::scene {

/**
 * @class CameraBase
 * @brief Protected non-virtual base for PerspectiveCamera and OrthographicCamera.
 *
 * Primary pose state is camera-to-world @c orientation_ plus @c look_distance_;
 * @c getTarget() is derived. @c up_hint_ is the stored look-at hint returned by
 * @c getUp().
 */
class VNE_SCENE_API CameraBase {
   protected:
    CameraBase() = default;
    explicit CameraBase(std::string name) noexcept
        : name_(std::move(name)) {}
    ~CameraBase() noexcept = default;

    CameraBase(const CameraBase&) = default;
    CameraBase& operator=(const CameraBase&) = default;
    CameraBase(CameraBase&&) noexcept = default;
    CameraBase& operator=(CameraBase&&) noexcept = default;

    //--------------------------------------------------------------------------
    // Pose — lookAtImpl defined in camera_base.cpp
    //--------------------------------------------------------------------------

    /** @brief Unit back vector and look distance from eye→target, with degeneracy fallback to @a orientation_fallback.
     */
    [[nodiscard]] static std::pair<vne::math::Vec3f, float> resolveBackUnitAndLookDistance(
        const vne::math::Vec3f& eye,
        const vne::math::Vec3f& target,
        const vne::math::Quatf& orientation_fallback) noexcept;

    void lookAtImpl(const vne::math::Vec3f& position,
                    const vne::math::Vec3f& target,
                    const vne::math::Vec3f& up) noexcept;

    void lookAtImpl(const vne::math::Vec3f& target, const vne::math::Vec3f& up) noexcept;

    /**
     * @brief Derive camera-to-world orientation from unit back vector and up hint.
     *
     * Matches the orthonormal basis construction used by
     * vneinteraction::TrackballStrategy::syncFromCamera: @c right = up×back,
     * with @p fallback_up used when @c up_hint×back is degenerate (parallel).
     */
    [[nodiscard]] static vne::math::Quatf orientationFromPosBack(
        vne::math::Vec3f back_unit,
        vne::math::Vec3f up_hint,
        const vne::math::Vec3f& fallback_up = vne::math::Vec3f(0.0f, 1.0f, 0.0f)) noexcept;

    [[nodiscard]] vne::math::Mat4f viewFromQuaternion(vne::math::GraphicsApi api, float scene_scale) const noexcept;

    void setTargetImpl(const vne::math::Vec3f& target) noexcept;
    void setUpImpl(const vne::math::Vec3f& up) noexcept;

    void setOrientationViewImpl(const vne::math::Vec3f& pos, const vne::math::Quatf& q) noexcept;

    [[nodiscard]] vne::math::Vec3f forwardDirImpl() const noexcept { return -orientation_.getZAxis(); }
    [[nodiscard]] vne::math::Vec3f rightDirImpl() const noexcept { return orientation_.getXAxis(); }
    [[nodiscard]] vne::math::Vec3f upDirImpl() const noexcept { return orientation_.getYAxis(); }
    [[nodiscard]] vne::math::Vec3f targetImpl() const noexcept { return position_ + forwardDirImpl() * look_distance_; }

    /**
     * @brief Apply scene zoom in the image plane after the view matrix is built.
     *
     * Uses scale(s, s, 1) so view-space depth is not scaled; projection near/far stay consistent
     * with geometry (uniform scale(s,s,s) on the view would break clip-space depth).
     *
     * @param look_at_view View matrix from quaternion (or legacy look-at); no scene scale yet.
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

    vne::math::Vec3f position_{0.0f, 0.0f, 0.0f};                 //!< Eye position in world space.
    vne::math::Quatf orientation_{vne::math::Quatf::identity()};  //!< Camera-to-world rotation.
    float look_distance_ = 1.0f;                                  //!< Eye-to-derived-target distance.
    vne::math::Vec3f up_hint_{0.0f, 1.0f, 0.0f};                  //!< Stored hint; @c getUp() returns this.

    std::string name_;    //!< Camera name.
    bool active_ = true;  //!< Whether this camera is active for rendering.
    vne::math::GraphicsApi graphics_api_{vne::math::GraphicsApi::eOpenGL};  //!< Backend for view/projection.
    float scene_scale_ = 1.0f;  //!< XY zoom factor baked into the view (see composeViewWithSceneScale).

    mutable vne::math::Mat4f view_matrix_{vne::math::Mat4f::identity()};             //!< Cached view matrix.
    mutable vne::math::Mat4f projection_matrix_{vne::math::Mat4f::identity()};       //!< Cached projection matrix.
    mutable vne::math::Mat4f view_projection_matrix_{vne::math::Mat4f::identity()};  //!< Cached projection * view (P*V).
    mutable bool view_matrix_dirty_ = true;        //!< True when view matrix needs recomputation.
    mutable bool projection_matrix_dirty_ = true;  //!< True when projection matrix needs recomputation.
    mutable bool vp_matrix_dirty_ =
        true;  //!< True when VP (= projection * view, i.e. P*V) needs recompute (set by view/projection updates, cleared by updateMatrices()).
};

}  // namespace vne::scene
