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
#include <vertexnova/logging/logging.h>
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/projection_utils.h>

#include <algorithm>

namespace vne::scene {

using namespace vne::math;

namespace {

CREATE_VNE_LOGGER_CATEGORY("vnescene.perspective_camera");

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
    vp_matrix_dirty_ = false;
    VNE_LOG_INFO << "PerspectiveCamera \"" << name_ << "\" created (fov=" << fov_ << "deg, aspect=" << aspect_ratio_
                 << ", near=" << near_plane_ << ", far=" << far_plane_ << ")";
}

PerspectiveCamera::PerspectiveCamera(
    float fov, float width, float height, float near_plane, float far_plane, std::string name)
    : CameraBase(std::move(name))
    , fov_(fov)
    , aspect_ratio_(height > 0.0f ? width / height : 1.0f)
    , near_plane_(near_plane)
    , far_plane_(far_plane)
    , width_(width)
    , height_(height) {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
    VNE_LOG_INFO << "PerspectiveCamera \"" << name_ << "\" created (fov=" << fov_ << "deg, " << width_ << "x" << height_
                 << ", near=" << near_plane_ << ", far=" << far_plane_ << ")";
}

Vec3f PerspectiveCamera::getPosition() const noexcept {
    return position_;
}

void PerspectiveCamera::setPosition(const Vec3f& position) noexcept {
    position_ = position;
    view_matrix_dirty_ = true;
}

Vec3f PerspectiveCamera::getTarget() const noexcept {
    return targetImpl();
}

void PerspectiveCamera::setTarget(const Vec3f& target) noexcept {
    setTargetImpl(target);
}

Vec3f PerspectiveCamera::getUp() const noexcept {
    return up_hint_;
}

void PerspectiveCamera::setUp(const Vec3f& up) noexcept {
    setUpImpl(up);
}

Quatf PerspectiveCamera::getOrientation() const noexcept {
    return orientation_;
}

void PerspectiveCamera::setOrientationView(const Vec3f& position, const Quatf& orientation) noexcept {
    setOrientationViewImpl(position, orientation);
    // Only view changed — skip projection recompute (called every frame by Navigation3D).
    updateViewMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
}

Vec3f PerspectiveCamera::getForwardDir() const noexcept {
    return forwardDirImpl();
}

Vec3f PerspectiveCamera::getRightDir() const noexcept {
    return rightDirImpl();
}

Vec3f PerspectiveCamera::getUpDir() const noexcept {
    return upDirImpl();
}

Mat4f PerspectiveCamera::getViewMatrix() const noexcept {
    if (view_matrix_dirty_) {
        const_cast<PerspectiveCamera*>(this)->updateViewMatrix();
    }
    return view_matrix_;
}

void PerspectiveCamera::updateViewMatrixImpl() noexcept {
    view_matrix_ = viewFromQuaternion(graphics_api_, scene_scale_);
    view_matrix_dirty_ = false;
    vp_matrix_dirty_ = true;
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
    vp_matrix_dirty_ = true;
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
    if (view_matrix_dirty_ || projection_matrix_dirty_ || vp_matrix_dirty_) {
        const_cast<PerspectiveCamera*>(this)->updateMatrices();
    }
    return view_projection_matrix_;
}

void PerspectiveCamera::updateMatrices() noexcept {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
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
    if (fov < kMinFovDeg || fov > kMaxFovDeg) {
        VNE_LOG_WARN << "PerspectiveCamera \"" << name_ << "\": FOV " << fov << " clamped to [" << kMinFovDeg << ", "
                     << kMaxFovDeg << "]";
    }
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
    if (height > 0.0f) {
        aspect_ratio_ = width / height;
    } else {
        VNE_LOG_WARN << "PerspectiveCamera \"" << name_ << "\": resize height=" << height
                     << " <= 0, aspect ratio unchanged";
    }
    projection_matrix_dirty_ = true;
}

void PerspectiveCamera::lookAt(const Vec3f& position, const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(position, target, up);
}

void PerspectiveCamera::lookAt(const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(target, up);
}

void PerspectiveCamera::setSceneScale(float scale) noexcept {
    if (scale <= 0.0f) {
        VNE_LOG_WARN << "PerspectiveCamera \"" << name_ << "\": setSceneScale(" << scale << ") <= 0, clamped to "
                     << kMinSceneScale;
    }
    // Clamp to a small positive minimum to avoid singular or axis-flipped view matrices.
    scene_scale_ = std::max(kMinSceneScale, scale);
    view_matrix_dirty_ = true;
}

Mat4f PerspectiveCamera::getClipToScreenMatrix(float width, float height) const noexcept {
    return vne::math::clipToScreenMatrix(width, height, graphics_api_);
}

Vec3f PerspectiveCamera::getForward() const noexcept {
    return forwardDirImpl();
}

Vec3f PerspectiveCamera::getRight() const noexcept {
    return rightDirImpl();
}

void PerspectiveCamera::moveForward(float distance) noexcept {
    position_ = position_ + forwardDirImpl() * distance;
    view_matrix_dirty_ = true;
}

void PerspectiveCamera::moveRight(float distance) noexcept {
    position_ = position_ + rightDirImpl() * distance;
    view_matrix_dirty_ = true;
}

void PerspectiveCamera::moveUp(float distance) noexcept {
    position_ = position_ + upDirImpl() * distance;
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
                                              getHeight(),
                                              getForwardDir(),
                                              getRightDir(),
                                              getUpDir());
}

}  // namespace vne::scene
