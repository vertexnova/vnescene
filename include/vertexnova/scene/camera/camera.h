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
 * @file camera.h
 * @brief Base camera interface for the scene system.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include <vertexnova/math/core/core.h>
#include <memory>
#include <string>

namespace vne::scene {

/**
 * @class ICamera
 * @brief Base camera interface for all camera types.
 *
 * Defines view and projection matrix handling, position/target/up,
 * and active/name state. Implementations (e.g. PerspectiveCamera,
 * OrthographicCamera) provide concrete projection and update logic.
 */
class VNE_SCENE_API ICamera {
   public:
    virtual ~ICamera() noexcept = default;

    /** @brief Get camera position in world space. */
    [[nodiscard]] virtual vne::math::Vec3f getPosition() const noexcept = 0;
    /** @brief Set camera position. */
    virtual void setPosition(const vne::math::Vec3f& position) noexcept = 0;
    /** @brief Get look-at target point. */
    [[nodiscard]] virtual vne::math::Vec3f getTarget() const noexcept = 0;
    /** @brief Set look-at target. */
    virtual void setTarget(const vne::math::Vec3f& target) noexcept = 0;
    /** @brief Get up vector. */
    [[nodiscard]] virtual vne::math::Vec3f getUp() const noexcept = 0;
    /** @brief Set up vector. */
    virtual void setUp(const vne::math::Vec3f& up) noexcept = 0;

    /** @brief Get current view matrix (cached, updated by updateViewMatrix). */
    [[nodiscard]] virtual vne::math::Mat4f getViewMatrix() const noexcept = 0;
    /** @brief Recompute view matrix from position/target/up. */
    virtual void updateViewMatrix() noexcept = 0;
    /** @brief Get current projection matrix (cached, updated by updateProjectionMatrix). */
    [[nodiscard]] virtual vne::math::Mat4f getProjectionMatrix() const noexcept = 0;
    /** @brief Recompute projection matrix (e.g. after FOV or bounds change). */
    virtual void updateProjectionMatrix() noexcept = 0;
    /** @brief Get view * projection matrix. */
    [[nodiscard]] virtual vne::math::Mat4f getViewProjectionMatrix() const noexcept = 0;
    /** @brief Recompute both view and projection matrices. */
    virtual void updateMatrices() noexcept = 0;

    /** @brief Whether this camera is active (e.g. used for rendering). */
    [[nodiscard]] virtual bool isActive() const noexcept = 0;
    /** @brief Set active state. */
    virtual void setActive(bool active) noexcept = 0;
    /** @brief Get near clip plane distance. */
    [[nodiscard]] virtual float getNearPlane() const noexcept = 0;
    /** @brief Get far clip plane distance. */
    [[nodiscard]] virtual float getFarPlane() const noexcept = 0;
    /** @brief Set near clip plane distance. */
    virtual void setNearPlane(float near_plane) noexcept = 0;
    /** @brief Set far clip plane distance. */
    virtual void setFarPlane(float far_plane) noexcept = 0;
    /** @brief Set near and far clip planes at once. */
    virtual void setClipPlanes(float near_plane, float far_plane) noexcept = 0;
    /** @brief Get viewport/frustum width (0 if N/A). */
    [[nodiscard]] virtual float getWidth() const noexcept = 0;
    /** @brief Get viewport/frustum height (0 if N/A). */
    [[nodiscard]] virtual float getHeight() const noexcept = 0;
    /** @brief Resize viewport or frustum bounds; recomputes aspect ratio / ortho bounds. */
    virtual void resize(float width, float height) noexcept = 0;

    /**
     * @brief Atomic pose setter: set position, target, and up in one call.
     * Marks the view matrix dirty once instead of three times.
     */
    virtual void lookAt(const vne::math::Vec3f& position,
                        const vne::math::Vec3f& target,
                        const vne::math::Vec3f& up) noexcept = 0;
    /**
     * @brief Pose setter keeping the current position; updates target and up.
     * Marks the view matrix dirty once.
     */
    virtual void lookAt(const vne::math::Vec3f& target, const vne::math::Vec3f& up) noexcept = 0;

    /**
     * @brief Get the uniform scene-scale factor baked into the view matrix.
     * Used by vneinteraction's eSceneScale zoom mode.
     */
    [[nodiscard]] virtual float getSceneScale() const noexcept = 0;
    /**
     * @brief Set the uniform scene-scale factor; baked into the view matrix on next update.
     * A value of 1.0 (default) has no effect.
     */
    virtual void setSceneScale(float scale) noexcept = 0;

    /**
     * @brief Return the NDC→screen transform matrix for the given viewport size.
     * Equivalent to vne::math::clipToScreenMatrix(width, height, getGraphicsApi()).
     */
    [[nodiscard]] virtual vne::math::Mat4f getClipToScreenMatrix(float width, float height) const noexcept = 0;

    /** @brief Get graphics API used for view/projection matrices (OpenGL, Vulkan, Metal, etc.). */
    [[nodiscard]] virtual vne::math::GraphicsApi getGraphicsApi() const noexcept = 0;
    /** @brief Set graphics API; view and projection are rebuilt for the chosen backend. */
    virtual void setGraphicsApi(vne::math::GraphicsApi api) noexcept = 0;

    /** @brief Pack camera data for GPU (view, proj, viewProj, position, near/far, viewport). */
    [[nodiscard]] virtual CameraGpu toGpu() const noexcept = 0;

    /** @brief Get camera name. */
    [[nodiscard]] virtual const std::string& getName() const noexcept = 0;
    /** @brief Set camera name. */
    virtual void setName(const std::string& name) noexcept = 0;
};

}  // namespace vne::scene
