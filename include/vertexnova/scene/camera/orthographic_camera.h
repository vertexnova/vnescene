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
 * @file orthographic_camera.h
 * @brief Orthographic (parallel projection) camera implementation.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/camera_base.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class OrthographicCamera
 * @brief Orthographic camera with configurable frustum bounds.
 *
 * Projection is defined by left, right, bottom, top, near, and far planes.
 * Supports resize(width, height) for viewport-based bounds.
 * Inherits shared state from CameraBase.
 */
class VNE_SCENE_API OrthographicCamera : public ICamera, protected CameraBase {
   public:
    /**
     * @brief Construct from explicit frustum bounds.
     * @param left Left extent in world units.
     * @param right Right extent in world units.
     * @param bottom Bottom extent in world units.
     * @param top Top extent in world units.
     * @param near_plane Near clip plane.
     * @param far_plane Far clip plane.
     * @param name Camera name.
     */
    OrthographicCamera(float left,
                       float right,
                       float bottom,
                       float top,
                       float near_plane,
                       float far_plane,
                       std::string name = "OrthographicCamera");
    /**
     * @brief Construct centered orthographic camera from viewport size.
     * @param width Viewport width; bounds are (-width/2, width/2) horizontally.
     * @param height Viewport height; bounds are (-height/2, height/2) vertically.
     * @param near_plane Near clip plane.
     * @param far_plane Far clip plane.
     * @param name Camera name.
     */
    OrthographicCamera(
        float width, float height, float near_plane, float far_plane, std::string name = "OrthographicCamera");
    ~OrthographicCamera() noexcept override = default;

    /** @brief Get camera position in world space. */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override;
    /** @brief Set camera position (orientation unchanged; derived target moves with the eye). */
    void setPosition(const vne::math::Vec3f& position) noexcept override;
    /** @brief Get look-at target (derived from orientation and look distance). */
    [[nodiscard]] vne::math::Vec3f getTarget() const noexcept override;
    /** @brief Set look-at target (recomputes orientation and look distance). */
    void setTarget(const vne::math::Vec3f& target) noexcept override;
    /** @brief Get stored up hint (not necessarily equal to camera basis up). */
    [[nodiscard]] vne::math::Vec3f getUp() const noexcept override;
    /** @brief Set up hint and re-derive orientation for the current view direction. */
    void setUp(const vne::math::Vec3f& up) noexcept override;

    [[nodiscard]] vne::math::Quatf getOrientation() const noexcept override;
    void setOrientationView(const vne::math::Vec3f& position, const vne::math::Quatf& orientation) noexcept override;
    [[nodiscard]] vne::math::Vec3f getForwardDir() const noexcept override;
    [[nodiscard]] vne::math::Vec3f getRightDir() const noexcept override;
    [[nodiscard]] vne::math::Vec3f getUpDir() const noexcept override;

    /** @brief Get current view matrix. */
    [[nodiscard]] vne::math::Mat4f getViewMatrix() const noexcept override;
    /** @brief Recompute view matrix from quaternion pose, position, and scene scale. */
    void updateViewMatrix() noexcept override;
    /** @brief Get current projection matrix. */
    [[nodiscard]] vne::math::Mat4f getProjectionMatrix() const noexcept override;
    /** @brief Recompute projection matrix from frustum bounds. */
    void updateProjectionMatrix() noexcept override;
    /** @brief Get view * projection matrix. */
    [[nodiscard]] vne::math::Mat4f getViewProjectionMatrix() const noexcept override;
    /** @brief Recompute view and projection matrices. */
    void updateMatrices() noexcept override;

    /** @brief Whether this camera is active. */
    [[nodiscard]] bool isActive() const noexcept override;
    /** @brief Set active state. */
    void setActive(bool active) noexcept override;
    /** @brief Get camera name. */
    [[nodiscard]] const std::string& getName() const noexcept override;
    /** @brief Set camera name. */
    void setName(const std::string& name) noexcept override;

    /** @brief Get left frustum bound. */
    [[nodiscard]] float getLeft() const noexcept { return left_; }
    /** @brief Set left frustum bound. */
    void setLeft(float left) noexcept;
    /** @brief Get right frustum bound. */
    [[nodiscard]] float getRight() const noexcept { return right_; }
    /** @brief Set right frustum bound. */
    void setRight(float right) noexcept;
    /** @brief Get bottom frustum bound. */
    [[nodiscard]] float getBottom() const noexcept { return bottom_; }
    /** @brief Set bottom frustum bound. */
    void setBottom(float bottom) noexcept;
    /** @brief Get top frustum bound. */
    [[nodiscard]] float getTop() const noexcept { return top_; }
    /** @brief Set top frustum bound. */
    void setTop(float top) noexcept;
    /** @brief Get near clip plane. */
    [[nodiscard]] float getNearPlane() const noexcept override { return near_plane_; }
    /** @brief Set near clip plane. */
    void setNearPlane(float near_plane) noexcept override;
    /** @brief Get far clip plane. */
    [[nodiscard]] float getFarPlane() const noexcept override { return far_plane_; }
    /** @brief Set far clip plane. */
    void setFarPlane(float far_plane) noexcept override;
    /** @brief Set near and far clip planes at once. */
    void setClipPlanes(float near_plane, float far_plane) noexcept override;
    /** @brief Set all frustum bounds at once. */
    void setBounds(float left, float right, float bottom, float top, float near_plane, float far_plane) noexcept;
    /** @brief Resize ortho bounds from viewport width/height (centered). */
    void resize(float width, float height) noexcept override;
    /** @brief Get frustum width (right - left). */
    [[nodiscard]] float getWidth() const noexcept override { return right_ - left_; }
    /** @brief Get frustum height (top - bottom). */
    [[nodiscard]] float getHeight() const noexcept override { return top_ - bottom_; }
    /** @brief Get aspect ratio (width / height). */
    [[nodiscard]] float getAspectRatio() const noexcept;
    /** @brief Get camera projection type. */
    [[nodiscard]] CameraType getCameraType() const noexcept override { return CameraType::eOrthographic; }
    /** @brief Get graphics API for view/projection. */
    [[nodiscard]] vne::math::GraphicsApi getGraphicsApi() const noexcept override { return graphics_api_; }
    /** @brief Set graphics API; marks matrices dirty so they are rebuilt. */
    void setGraphicsApi(vne::math::GraphicsApi api) noexcept override;
    /** @brief Pack as CameraGpu for GPU. */
    [[nodiscard]] CameraGpu toGpu() const noexcept override;

    /** @brief Set position, target, and up atomically; one dirty mark. */
    void lookAt(const vne::math::Vec3f& position,
                const vne::math::Vec3f& target,
                const vne::math::Vec3f& up) noexcept override;
    /** @brief Keep current position; set target and up; one dirty mark. */
    void lookAt(const vne::math::Vec3f& target, const vne::math::Vec3f& up) noexcept override;

    /** @brief XY scene zoom factor baked into the view matrix (after clamping; see ICamera::setSceneScale). */
    [[nodiscard]] float getSceneScale() const noexcept override { return scene_scale_; }
    /** @brief Set XY scene zoom; clamps to minimum 1e-4; marks view matrix dirty only. */
    void setSceneScale(float scale) noexcept override;

    /** @brief Return NDC→screen matrix for the given viewport size. */
    [[nodiscard]] vne::math::Mat4f getClipToScreenMatrix(float width, float height) const noexcept override;

   private:
    /** @brief Non-virtual implementation to avoid virtual call during construction. */
    void updateViewMatrixImpl() noexcept;
    /** @brief Non-virtual implementation to avoid virtual call during construction. */
    void updateProjectionMatrixImpl() noexcept;

    float left_;        //!< Left frustum bound (world units).
    float right_;       //!< Right frustum bound (world units).
    float bottom_;      //!< Bottom frustum bound (world units).
    float top_;         //!< Top frustum bound (world units).
    float near_plane_;  //!< Near clip plane distance.
    float far_plane_;   //!< Far clip plane distance.
};

}  // namespace vne::scene
