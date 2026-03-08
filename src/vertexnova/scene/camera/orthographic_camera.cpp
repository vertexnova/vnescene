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

#include <algorithm>

namespace vne::scene {

using namespace vne::math;

OrthographicCamera::OrthographicCamera(
    float left, float right, float bottom, float top, float near_plane, float far_plane, const std::string& name)
    : name_(name)
    , left_(left)
    , right_(right)
    , bottom_(bottom)
    , top_(top)
    , near_plane_(near_plane)
    , far_plane_(far_plane) {
    OrthographicCamera::updateMatrices();
}

OrthographicCamera::OrthographicCamera(
    float width, float height, float near_plane, float far_plane, const std::string& name)
    : name_(name)
    , left_(-width * 0.5f)
    , right_(width * 0.5f)
    , bottom_(-height * 0.5f)
    , top_(height * 0.5f)
    , near_plane_(near_plane)
    , far_plane_(far_plane) {
    OrthographicCamera::updateMatrices();
}

Vec3f OrthographicCamera::getPosition() const noexcept {
    return position_;
}

void OrthographicCamera::setPosition(const Vec3f& position) noexcept {
    position_ = position;
    view_matrix_dirty_ = true;
}

Vec3f OrthographicCamera::getTarget() const noexcept {
    return target_;
}

void OrthographicCamera::setTarget(const Vec3f& target) noexcept {
    target_ = target;
    view_matrix_dirty_ = true;
}

Vec3f OrthographicCamera::getUp() const noexcept {
    return up_;
}

void OrthographicCamera::setUp(const Vec3f& up) noexcept {
    up_ = up;
    view_matrix_dirty_ = true;
}

Mat4f OrthographicCamera::getViewMatrix() const noexcept {
    if (view_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateViewMatrix();
    }
    return view_matrix_;
}

void OrthographicCamera::updateViewMatrix() noexcept {
    view_matrix_ = Mat4f::lookAt(position_, target_, up_, graphics_api_);
    view_matrix_dirty_ = false;
}

Mat4f OrthographicCamera::getProjectionMatrix() const noexcept {
    if (projection_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateProjectionMatrix();
    }
    return projection_matrix_;
}

void OrthographicCamera::updateProjectionMatrix() noexcept {
    projection_matrix_ = Mat4f::ortho(left_, right_, bottom_, top_, near_plane_, far_plane_, graphics_api_);
    projection_matrix_dirty_ = false;
}
void OrthographicCamera::setGraphicsApi(GraphicsApi api) noexcept {
    if (graphics_api_ != api) {
        graphics_api_ = api;
        view_matrix_dirty_ = true;
        projection_matrix_dirty_ = true;
    }
}
Mat4f OrthographicCamera::getViewProjectionMatrix() const noexcept {
    if (view_matrix_dirty_ || projection_matrix_dirty_) {
        const_cast<OrthographicCamera*>(this)->updateMatrices();
    }
    return view_projection_matrix_;
}

void OrthographicCamera::updateMatrices() noexcept {
    updateViewMatrix();
    updateProjectionMatrix();
    view_projection_matrix_ = projection_matrix_ * view_matrix_;
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
    near_plane_ = std::max(0.001f, near_plane);
    if (near_plane_ >= far_plane_) {
        far_plane_ = near_plane_ + 1.0f;
    }
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setFarPlane(float far_plane) noexcept {
    far_plane_ = std::max(near_plane_ + 0.1f, far_plane);
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::setBounds(
    float left, float right, float bottom, float top, float near_plane, float far_plane) noexcept {
    left_ = left;
    right_ = right;
    bottom_ = bottom;
    top_ = top;
    near_plane_ = near_plane;
    far_plane_ = far_plane;
    projection_matrix_dirty_ = true;
}

void OrthographicCamera::resize(float width, float height) noexcept {
    float half_w = width * 0.5f;
    float half_h = height * 0.5f;
    left_ = -half_w;
    right_ = half_w;
    bottom_ = -half_h;
    top_ = half_h;
    projection_matrix_dirty_ = true;
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
                                              getHeight());
}

}  // namespace vne::scene
