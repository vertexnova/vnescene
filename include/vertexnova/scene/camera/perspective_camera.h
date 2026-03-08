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
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class PerspectiveCamera
 * @brief Perspective camera with FOV and aspect ratio.
 *
 * Supports viewport resize, lookAt, and orbit-style rotation around target
 * (rotateAroundTarget). Forward/right/up helpers for first-person style movement.
 */
class VNE_SCENE_API PerspectiveCamera : public ICamera {
   public:
    /**
     * @brief Construct from FOV and aspect ratio.
     * @param fov Field of view in degrees (e.g. vertical).
     * @param aspect_ratio Width / height.
     * @param near_plane Near clip plane.
     * @param far_plane Far clip plane.
     * @param name Camera name.
     */
    PerspectiveCamera(float fov,
                      float aspect_ratio,
                      float near_plane,
                      float far_plane,
                      const std::string& name = "PerspectiveCamera");
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
                      const std::string& name = "PerspectiveCamera");
    ~PerspectiveCamera() noexcept override = default;

    /** @brief Get camera position in world space. */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override;
    /** @brief Set camera position. */
    void setPosition(const vne::math::Vec3f& position) noexcept override;
    /** @brief Get look-at target. */
    [[nodiscard]] vne::math::Vec3f getTarget() const noexcept override;
    /** @brief Set look-at target. */
    void setTarget(const vne::math::Vec3f& target) noexcept override;
    /** @brief Get up vector. */
    [[nodiscard]] vne::math::Vec3f getUp() const noexcept override;
    /** @brief Set up vector. */
    void setUp(const vne::math::Vec3f& up) noexcept override;

    /** @brief Get current view matrix. */
    [[nodiscard]] vne::math::Mat4f getViewMatrix() const noexcept override;
    /** @brief Recompute view matrix from position/target/up. */
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
    void setNearPlane(float near_plane) noexcept;
    /** @brief Get far clip plane. */
    [[nodiscard]] float getFarPlane() const noexcept override { return far_plane_; }
    /** @brief Set far clip plane. */
    void setFarPlane(float far_plane) noexcept;
    /** @brief Set FOV, aspect ratio, and clip planes at once. */
    void setPerspective(float fov, float aspect_ratio, float near_plane, float far_plane) noexcept;
    /** @brief Resize viewport and recompute aspect ratio. */
    void resize(float width, float height) noexcept;
    /** @brief Get viewport width. */
    [[nodiscard]] float getWidth() const noexcept override { return width_; }
    /** @brief Get viewport height. */
    [[nodiscard]] float getHeight() const noexcept override { return height_; }
    /** @brief Set viewport size and recompute aspect ratio. */
    void setViewport(float width, float height) noexcept;
    /** @brief Get graphics API for view/projection. */
    [[nodiscard]] vne::math::GraphicsApi getGraphicsApi() const noexcept override { return graphics_api_; }
    /** @brief Set graphics API; marks matrices dirty so they are rebuilt. */
    void setGraphicsApi(vne::math::GraphicsApi api) noexcept override;
    /** @brief Pack as CameraGpu for GPU. */
    [[nodiscard]] CameraGpu toGpu() const noexcept override;

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
    /** @brief Orbit around current target by yaw and pitch angles (degrees). */
    void rotateAroundTarget(float yaw_angle, float pitch_angle) noexcept;
    /** @brief Set target and up; recompute view. */
    void lookAt(const vne::math::Vec3f& target,
                const vne::math::Vec3f& up = vne::math::Vec3f(0.0f, 1.0f, 0.0f)) noexcept;

   private:
    vne::math::Vec3f position_{0.0f, 0.0f, 0.0f};  //!< Camera position in world space.
    vne::math::Vec3f target_{0.0f, 0.0f, -1.0f};   //!< Look-at target point.
    vne::math::Vec3f up_{0.0f, 1.0f, 0.0f};        //!< Up vector for view orientation.
    std::string name_;                             //!< Camera name.
    bool active_ = true;                           //!< Whether this camera is active for rendering.
    vne::math::GraphicsApi graphics_api_{vne::math::GraphicsApi::eOpenGL};  //!< Backend for view/projection.
    float fov_;                                                             //!< Field of view in degrees.
    float aspect_ratio_;                                                    //!< Viewport aspect ratio (width / height).
    float near_plane_;                                                      //!< Near clip plane distance.
    float far_plane_;                                                       //!< Far clip plane distance.
    float width_ = 0.0f;                                                    //!< Viewport width (for aspect/resize).
    float height_ = 0.0f;                                                   //!< Viewport height (for aspect/resize).
    mutable vne::math::Mat4f view_matrix_{vne::math::Mat4f::identity()};    //!< Cached view matrix.
    mutable vne::math::Mat4f projection_matrix_{vne::math::Mat4f::identity()};       //!< Cached projection matrix.
    mutable vne::math::Mat4f view_projection_matrix_{vne::math::Mat4f::identity()};  //!< Cached view * projection.
    mutable bool view_matrix_dirty_ = true;        //!< True when view matrix needs recomputation.
    mutable bool projection_matrix_dirty_ = true;  //!< True when projection matrix needs recomputation.
};

}  // namespace vne::scene
