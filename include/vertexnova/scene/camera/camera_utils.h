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
 * @file camera_utils.h
 * @brief Camera-domain helpers: frustum, screen ray, fit-to-AABB (uses vnemath).
 *
 * Ray/Frustum/AABB live in vnemath; these wrappers take ICamera for picking,
 * focus, and debug. No input or GPU ownership.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/perspective_camera.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/geometry/frustum.h>
#include <vertexnova/math/geometry/ray.h>
#include <vertexnova/math/geometry/aabb.h>
#include <vertexnova/math/projection_utils.h>
#include <vertexnova/math/viewport.h>

namespace vne::scene {

/**
 * @brief Build world-space frustum from camera view-projection.
 * @param camera The camera (view and projection taken from it).
 * @return Frustum in world space (extracted from view * projection).
 */
[[nodiscard]] inline vne::math::Frustum buildFrustum(const ICamera& camera) noexcept {
    vne::math::Mat4f vp = camera.getViewProjectionMatrix();
    vne::math::Frustum f;
    f.extractFromMatrix(vp);
    return f;
}

/**
 * @brief Build a picking ray from screen pixel coordinates.
 * @param camera The camera (position and view-projection used).
 * @param x_px Screen x in pixels.
 * @param y_px Screen y in pixels.
 * @param viewport_width Viewport width in pixels.
 * @param viewport_height Viewport height in pixels.
 * @param api Graphics API for Y-flip (default OpenGL).
 * @return World-space ray from camera through the screen point.
 */
[[nodiscard]] inline vne::math::Ray screenToWorldRay(
    const ICamera& camera,
    float x_px,
    float y_px,
    float viewport_width,
    float viewport_height,
    vne::math::GraphicsApi api = vne::math::GraphicsApi::eOpenGL) noexcept {
    vne::math::Mat4f vp = camera.getViewProjectionMatrix();
    vne::math::Mat4f inv_vp = vp.inverse();
    vne::math::Viewport vp_port(viewport_width, viewport_height);
    return vne::math::screenToWorldRay(vne::math::Vec2f(x_px, y_px), inv_vp, vp_port, camera.getPosition(), api);
}

/**
 * @brief Fit a perspective camera to fully contain an AABB with padding.
 * Places camera so the AABB is in view; adjusts distance and optional FOV to fit.
 * @param cam The perspective camera to modify (position, target, optionally FOV).
 * @param aabb World-space AABB to frame.
 * @param padding Scale factor around the AABB (e.g. 1.1f = 10% margin).
 */
VNE_SCENE_API void fitToAabb(PerspectiveCamera& cam, const vne::math::Aabb& aabb, float padding = 1.1f) noexcept;

/**
 * @brief Project a world position to screen (x, y in pixels).
 * @param camera The camera.
 * @param world_pos World-space point.
 * @param viewport_width Viewport width in pixels.
 * @param viewport_height Viewport height in pixels.
 * @param api Graphics API (default OpenGL).
 * @return Screen (x, y) and depth component.
 */
[[nodiscard]] inline vne::math::Vec3f project(const ICamera& camera,
                                              const vne::math::Vec3f& world_pos,
                                              float viewport_width,
                                              float viewport_height,
                                              vne::math::GraphicsApi api = vne::math::GraphicsApi::eOpenGL) noexcept {
    vne::math::Mat4f vp = camera.getViewProjectionMatrix();
    return vne::math::project(world_pos, vp, vne::math::Viewport(viewport_width, viewport_height), api);
}

/**
 * @brief Unproject screen (x, y) and depth to world position.
 * @param camera The camera.
 * @param screen_pos Screen (x, y) and depth (e.g. 0 = near, 1 = far).
 * @param viewport_width Viewport width in pixels.
 * @param viewport_height Viewport height in pixels.
 * @param api Graphics API (default OpenGL).
 * @return World-space position.
 */
[[nodiscard]] inline vne::math::Vec3f unproject(const ICamera& camera,
                                                const vne::math::Vec3f& screen_pos,
                                                float viewport_width,
                                                float viewport_height,
                                                vne::math::GraphicsApi api = vne::math::GraphicsApi::eOpenGL) noexcept {
    vne::math::Mat4f vp = camera.getViewProjectionMatrix();
    vne::math::Mat4f inv_vp = vp.inverse();
    return vne::math::unproject(screen_pos, inv_vp, vne::math::Viewport(viewport_width, viewport_height), api);
}

}  // namespace vne::scene
