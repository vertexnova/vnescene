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

#include "vertexnova/scene/camera/orthographic_camera.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/projection_utils.h>

#include <algorithm>

namespace vne::scene {

using namespace vne::math;

namespace {

constexpr float kHalf = 0.5f;
constexpr float kMinNearPlane = 0.001f;
constexpr float kMinFarPlaneOffset = 0.1f;
constexpr float kMinSceneScale = 1e-4f;
constexpr float kMinDimension = 1e-3f;  // Minimum width/height to avoid degenerate projection.

}  // namespace

OrthographicCamera::OrthographicCamera(
    float left, float right, float bottom, float top, float near_plane, float far_plane, std::string name)
    : CameraBase(std::move(name))
    , left_(std::min(left, right - kMinDimension))
    , right_(std::max(right, left + kMinDimension))
    , bottom_(std::min(bottom, top - kMinDimension))
    , top_(std::max(top, bottom + kMinDimension))
    , near_plane_(std::max(kMinNearPlane, near_plane))
    , far_plane_(std::max(std::max(kMinNearPlane, near_plane) + kMinFarPlaneOffset, far_plane)) {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
}

OrthographicCamera::OrthographicCamera(float width, float height, float near_plane, float far_plane, std::string name)
    : CameraBase(std::move(name))
    , left_(-width * kHalf)
    , right_(width * kHalf)
    , bottom_(-height * kHalf)
    , top_(height * kHalf)
    , near_plane_(near_plane)
    , far_plane_(far_plane) {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
}

Vec3f OrthographicCamera::getPosition() const noexcept {
    return position_;
}

void OrthographicCamera::setPosition(const Vec3f& position) noexcept {
    position_ = position;
    view_matrix_dirty_ = true;
}

Vec3f OrthographicCamera::getTarget() const noexcept {
    return targetImpl();
}

void OrthographicCamera::setTarget(const Vec3f& target) noexcept {
    setTargetImpl(target);
}

Vec3f OrthographicCamera::getUp() const noexcept {
    return up_hint_;
}

void OrthographicCamera::setUp(const Vec3f& up) noexcept {
    setUpImpl(up);
}

Quatf OrthographicCamera::getOrientation() const noexcept {
    return orientation_;
}

void OrthographicCamera::setOrientationView(const Vec3f& position, const Quatf& orientation) noexcept {
    setOrientationViewImpl(position, orientation);
    // Only view changed — skip projection recompute.
    updateViewMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
}

Vec3f OrthographicCamera::getForwardDir() const noexcept {
    return forwardDirImpl();
}

Vec3f OrthographicCamera::getRightDir() const noexcept {
    return rightDirImpl();
}

Vec3f OrthographicCamera::getUpDir() const noexcept {
    return upDirImpl();
}

Mat4f OrthographicCamera::getViewMatrix() const noexcept {
    if (view_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateViewMatrix();
    }
    return view_matrix_;
}

void OrthographicCamera::updateViewMatrixImpl() noexcept {
    view_matrix_ = viewFromQuaternion(graphics_api_, scene_scale_);
    view_matrix_dirty_ = false;
    vp_matrix_dirty_ = true;
}

void OrthographicCamera::updateViewMatrix() noexcept {
    updateViewMatrixImpl();
}

Mat4f OrthographicCamera::getProjectionMatrix() const noexcept {
    if (projection_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateProjectionMatrix();
    }
    return projection_matrix_;
}

void OrthographicCamera::updateProjectionMatrixImpl() noexcept {
    projection_matrix_ = Mat4f::ortho(left_, right_, bottom_, top_, near_plane_, far_plane_, graphics_api_);
    projection_matrix_dirty_ = false;
    vp_matrix_dirty_ = true;
}

void OrthographicCamera::updateProjectionMatrix() noexcept {
    updateProjectionMatrixImpl();
}

void OrthographicCamera::setGraphicsApi(GraphicsApi api) noexcept {
    if (graphics_api_ != api) {
        graphics_api_ = api;
        view_matrix_dirty_ = true;
        projection_matrix_dirty_ = true;
    }
}

Mat4f OrthographicCamera::getViewProjectionMatrix() const noexcept {
    if (view_matrix_dirty_ || projection_matrix_dirty_ || vp_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateMatrices();
    }
    return view_projection_matrix_;
}

void OrthographicCamera::updateMatrices() noexcept {
    updateViewMatrixImpl();
    updateProjectionMatrixImpl();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
    vp_matrix_dirty_ = false;
}

bool OrthographicCamera::isActive() const noexcept {
    return active_;
}

void OrthographicCamera::setActive(bool active) noexcept {
    active_ = active;
}

const std::string& OrthographicCamera::getName() const noexcept {
    return name_;
}

void OrthographicCamera::setName(const std::string& name) noexcept {
    name_ = name;
}

void OrthographicCamera::setLeft(float left) noexcept {
    left_ = left;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setRight(float right) noexcept {
    right_ = right;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setBottom(float bottom) noexcept {
    bottom_ = bottom;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setTop(float top) noexcept {
    top_ = top;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setNearPlane(float near_plane) noexcept {
    near_plane_ = std::max(kMinNearPlane, near_plane);
    if (near_plane_ >= far_plane_) {
        far_plane_ = near_plane_ + 1.0f;
    }
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setFarPlane(float far_plane) noexcept {
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setClipPlanes(float near_plane, float far_plane) noexcept {
    near_plane_ = std::max(kMinNearPlane, near_plane);
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setBounds(
    float left, float right, float bottom, float top, float near_plane, float far_plane) noexcept {
    if (right <= left) {
        float mid = (left + right) * kHalf;
        left_ = mid - kMinDimension * kHalf;
        right_ = mid + kMinDimension * kHalf;
    } else {
        left_ = left;
        right_ = right;
    }
    if (top <= bottom) {
        float mid = (bottom + top) * kHalf;
        bottom_ = mid - kMinDimension * kHalf;
        top_ = mid + kMinDimension * kHalf;
    } else {
        bottom_ = bottom;
        top_ = top;
    }
    near_plane_ = std::max(kMinNearPlane, near_plane);
    far_plane_ = std::max(near_plane_ + kMinFarPlaneOffset, far_plane);
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::resize(float width, float height) noexcept {
    float w = std::max(kMinDimension, width);
    float h = std::max(kMinDimension, height);
    float half_w = w * kHalf;
    float half_h = h * kHalf;
    left_ = -half_w;
    right_ = half_w;
    bottom_ = -half_h;
    top_ = half_h;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::lookAt(const Vec3f& position, const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(position, target, up);
}

void OrthographicCamera::lookAt(const Vec3f& target, const Vec3f& up) noexcept {
    lookAtImpl(target, up);
}

void OrthographicCamera::setSceneScale(float scale) noexcept {
    // Clamp to a small positive minimum to avoid singular or axis-flipped view matrices.
    scene_scale_ = std::max(kMinSceneScale, scale);
    view_matrix_dirty_ = true;
}

Mat4f OrthographicCamera::getClipToScreenMatrix(float width, float height) const noexcept {
    return vne::math::clipToScreenMatrix(width, height, graphics_api_);
}

float OrthographicCamera::getAspectRatio() const noexcept {
    float w = right_ - left_;
    float h = top_ - bottom_;
    return (h > 0.0f) ? (w / h) : 1.0f;
}

CameraGpu OrthographicCamera::toGpu() const noexcept {
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
