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
 * @file perspective_camera.h
 * @brief Perspective (FOV-based) camera implementation.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/camera_base.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class PerspectiveCamera
 * @brief Perspective camera with FOV and aspect ratio.
 *
 * Supports viewport resize, lookAt, and forward/right/up movement helpers.
 * Inherits shared state from CameraBase.
 */
class VNE_SCENE_API PerspectiveCamera : public ICamera, protected CameraBase {
   public:
    /**
     * @brief Construct from FOV and aspect ratio.
     * @param fov Field of view in degrees (e.g. vertical).
     * @param aspect_ratio Width / height.
     * @param near_plane Near clip plane.
     * @param far_plane Far clip plane.
     * @param name Camera name.
     */
    PerspectiveCamera(
        float fov, float aspect_ratio, float near_plane, float far_plane, std::string name = "PerspectiveCamera");
    /**
     * @brief Construct from FOV and viewport size (aspect computed).
     * @param fov Field of view in degrees.
     * @param width Viewport width.
     * @param height Viewport height.
     * @param near_plane Near clip plane.
     * @param far_plane Far clip plane.
     * @param name Camera name.
     */
    PerspectiveCamera(float fov,
                      float width,
                      float height,
                      float near_plane,
                      float far_plane,
                      std::string name = "PerspectiveCamera");
    ~PerspectiveCamera() noexcept override = default;

    /** @brief Get camera position in world space. */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override;
    /** @brief Set camera position (orientation unchanged; derived target moves with the eye). */
    void setPosition(const vne::math::Vec3f& position) noexcept override;
    /** @brief Get look-at target (derived from orientation and look distance). */
    [[nodiscard]] vne::math::Vec3f getTarget() const noexcept override;
    /** @brief Set look-at target (recomputes orientation and look distance). */
    void setTarget(const vne::math::Vec3f& target) noexcept override;
    /** @brief Stored up hint used to reconstruct orientation (see @c orientationFromPosBack). */
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
    /** @brief Recompute projection matrix from FOV and aspect. */
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

    /** @brief Get field of view in degrees. */
    [[nodiscard]] float getFieldOfView() const noexcept { return fov_; }
    /** @brief Set field of view in degrees. */
    void setFieldOfView(float fov) noexcept;
    /** @brief Get aspect ratio (width / height). */
    [[nodiscard]] float getAspectRatio() const noexcept { return aspect_ratio_; }
    /** @brief Set aspect ratio. */
    void setAspectRatio(float aspect_ratio) noexcept;
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
    /** @brief Set FOV, aspect ratio, and clip planes at once. */
    void setPerspective(float fov, float aspect_ratio, float near_plane, float far_plane) noexcept;
    /** @brief Resize viewport and recompute aspect ratio. */
    void resize(float width, float height) noexcept override;
    /** @brief Get viewport width. */
    [[nodiscard]] float getWidth() const noexcept override { return width_; }
    /** @brief Get viewport height. */
    [[nodiscard]] float getHeight() const noexcept override { return height_; }
    /** @brief Get camera projection type. */
    [[nodiscard]] CameraType getCameraType() const noexcept override { return CameraType::ePerspective; }
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

    /** @brief Forward vector (target - position, normalized). */
    [[nodiscard]] vne::math::Vec3f getForward() const noexcept;
    /** @brief Right vector (cross(forward, up), normalized). */
    [[nodiscard]] vne::math::Vec3f getRight() const noexcept;
    /** @brief Move camera along forward by distance. */
    void moveForward(float distance) noexcept;
    /** @brief Move camera along right by distance. */
    void moveRight(float distance) noexcept;
    /** @brief Move camera along up by distance. */
    void moveUp(float distance) noexcept;

   private:
    /** @brief Non-virtual implementation to avoid virtual call during construction. */
    void updateViewMatrixImpl() noexcept;
    /** @brief Non-virtual implementation to avoid virtual call during construction. */
    void updateProjectionMatrixImpl() noexcept;

    float fov_ = 60.0f;                  //!< Field of view in degrees.
    float aspect_ratio_ = 16.0f / 9.0f;  //!< Viewport aspect ratio (width / height).
    float near_plane_ = 0.1f;            //!< Near clip plane distance.
    float far_plane_ = 1000.0f;          //!< Far clip plane distance.
    float width_ = 0.0f;                 //!< Viewport width (for aspect/resize).
    float height_ = 0.0f;                //!< Viewport height (for aspect/resize).
};

}  // namespace vne::scene
