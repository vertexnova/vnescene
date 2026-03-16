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

#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/projection_utils.h>

#include <algorithm>

namespace vne::scene {

using namespace vne::math;

namespace {

constexpr float kMinFovDeg = 1.0f;
constexpr float kMaxFovDeg = 179.0f;
constexpr float kMinAspectRatio = 0.1f;
constexpr float kMinNearPlane = 0.001f;
constexpr float kMinFarPlaneOffset = 0.1f;
constexpr float kMinSceneScale = 1e-4f;

}  // namespace

PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, float near_plane, float far_plane, std::string name)
    : CameraBase(std::move(name))
    , fov_(fov)
    , aspect_ratio_(aspect_ratio)
    , near_plane_(near_plane)
    , far_plane_(far_plane) {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

PerspectiveCamera::PerspectiveCamera(
    float fov, float width, float height, float near_plane, float far_plane, std::string name)
    : CameraBase(std::move(name))
    , fov_(fov)
    , aspect_ratio_(width / height)
    , near_plane_(near_plane)
    , far_plane_(far_plane)
    , width_(width)
    , height_(height) {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

Vec3f PerspectiveCamera::getPosition() const noexcept {
    return position_;
}

void PerspectiveCamera::setPosition(const Vec3f& position) noexcept {
    position_ = position;
    view_matrix_dirty_ = true;
}

Vec3f PerspectiveCamera::getTarget() const noexcept {
    return target_;
}

void PerspectiveCamera::setTarget(const Vec3f& target) noexcept {
    target_ = target;
    view_matrix_dirty_ = true;
}

Vec3f PerspectiveCamera::getUp() const noexcept {
    return up_;
}

void PerspectiveCamera::setUp(const Vec3f& up) noexcept {
    up_ = up;
    view_matrix_dirty_ = true;
}

Mat4f PerspectiveCamera::getViewMatrix() const noexcept {
    if (view_matrix_dirty_) {
        const_cast<PerspectiveCamera*>(this)->updateViewMatrix();
    }
    return view_matrix_;
}

void PerspectiveCamera::updateViewMatrixImpl() noexcept {
    view_matrix_ = Mat4f::lookAt(position_, target_, up_, graphics_api_);
    if (scene_scale_ != 1.0f) {
        view_matrix_ = Mat4f::scale(scene_scale_, scene_scale_, scene_scale_) * view_matrix_;
    }
    view_matrix_dirty_ = false;
}

void PerspectiveCamera::updateViewMatrix() noexcept {
    updateViewMatrixImpl();
}

Mat4f PerspectiveCamera::getProjectionMatrix() const noexcept {
    if (projection_matrix_dirty_) {
        const_cast<PerspectiveCamera*>(this)->updateProjectionMatrix();
    }
    return projection_matrix_;
}

void PerspectiveCamera::updateProjectionMatrixImpl() noexcept {
    float fov_rad = degToRad(fov_);
    projection_matrix_ = Mat4f::perspective(fov_rad, aspect_ratio_, near_plane_, far_plane_, graphics_api_);
    projection_matrix_dirty_ = false;
}

void PerspectiveCamera::updateProjectionMatrix() noexcept {
    updateProjectionMatrixImpl();
}

void PerspectiveCamera::setGraphicsApi(GraphicsApi api) noexcept {
    if (graphics_api_ != api) {
        graphics_api_ = api;
        view_matrix_dirty_ = true;
        projection_matrix_dirty_ = true;
    }
}

Mat4f PerspectiveCamera::getViewProjectionMatrix() const noexcept {
    if (view_matrix_dirty_ || projection_matrix_dirty_) {
        const_cast<PerspectiveCamera*>(this)->updateMatrices();
    }
    return view_projection_matrix_;
}

void PerspectiveCamera::updateMatrices() noexcept {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

bool PerspectiveCamera::isActive() const noexcept {
    return active_;
}

void PerspectiveCamera::setActive(bool active) noexcept {
    active_ = active;
}

const std::string& PerspectiveCamera::getName() const noexcept {
    return name_;
}

void PerspectiveCamera::setName(const std::string& name) noexcept {
    name_ = name;
}

void PerspectiveCamera::setFieldOfView(float fov) noexcept {
    fov_ = std::clamp(fov, kMinFovDeg, kMaxFovDeg);
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::setAspectRatio(float aspect_ratio) noexcept {
    aspect_ratio_ = std::max(kMinAspectRatio, aspect_ratio);
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::setNearPlane(float near_plane) noexcept {
    near_plane_ = std::max(kMinNearPlane, near_plane);
    if (near_plane_ >= far_plane_) {
        far_plane_ = near_plane_ + 1.0f;
    }
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::setFarPlane(float far_plane) noexcept {
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::setClipPlanes(float near_plane, float far_plane) noexcept {
    near_plane_ = std::max(kMinNearPlane, near_plane);
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::setPerspective(float fov, float aspect_ratio, float near_plane, float far_plane) noexcept {
    fov_ = std::clamp(fov, kMinFovDeg, kMaxFovDeg);
    aspect_ratio_ = std::max(kMinAspectRatio, aspect_ratio);
    near_plane_ = std::max(kMinNearPlane, near_plane);
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::resize(float width, float height) noexcept {
    width_ = width;
    height_ = height;
    aspect_ratio_ = width / height;
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::lookAt(const Vec3f& position, const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(position, target, up);
}

void PerspectiveCamera::lookAt(const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(target, up);
}

void PerspectiveCamera::setSceneScale(float scale) noexcept {
    // Clamp to a small positive minimum to avoid singular or axis-flipped view matrices.
    scene_scale_ = std::max(kMinSceneScale, scale);
    view_matrix_dirty_ = true;
}

Mat4f PerspectiveCamera::getClipToScreenMatrix(float width, float height) const noexcept {
    return vne::math::clipToScreenMatrix(width, height, graphics_api_);
}

Vec3f PerspectiveCamera::getForward() const noexcept {
    Vec3f forward = target_ - position_;
    return forward.normalized();
}

Vec3f PerspectiveCamera::getRight() const noexcept {
    Vec3f forward = getForward();
    return forward.cross(up_).normalized();
}

void PerspectiveCamera::moveForward(float distance) noexcept {
    Vec3f forward = getForward();
    position_ = position_ + forward * distance;
    target_ = target_ + forward * distance;
    view_matrix_dirty_ = true;
}

void PerspectiveCamera::moveRight(float distance) noexcept {
    Vec3f right = getRight();
    position_ = position_ + right * distance;
    target_ = target_ + right * distance;
    view_matrix_dirty_ = true;
}

void PerspectiveCamera::moveUp(float distance) noexcept {
    position_ = position_ + up_ * distance;
    target_ = target_ + up_ * distance;
    view_matrix_dirty_ = true;
}

void PerspectiveCamera::rotateAroundTarget(float yaw_angle, float pitch_angle) noexcept {
    float yaw_rad = degToRad(yaw_angle);
    float pitch_rad = degToRad(pitch_angle);
    Vec3f direction = position_ - target_;
    float dist = direction.length();
    Mat4f yaw_rot = Mat4f::rotateY(yaw_rad);
    Mat4f pitch_rot = Mat4f::rotateX(pitch_rad);
    Mat4f combined = pitch_rot * yaw_rot;
    Vec3f rotated_dir = combined.transformVector(direction);
    position_ = target_ + rotated_dir.normalized() * dist;
    view_matrix_dirty_ = true;
}

CameraGpu PerspectiveCamera::toGpu() const noexcept {
    Mat4f view = getViewMatrix();
    Mat4f proj = getProjectionMatrix();
    Mat4f vp = getViewProjectionMatrix();
    Vec3f pos = getPosition();
    return makeCameraGpuFromMatricesAndParams(view,
                                              proj,
                                              vp,
                                              pos,
                                              getNearPlane(),
                                              getFarPlane(),
                                              getWidth(),
                                              getHeight());
}

}  // namespace vne::scene
