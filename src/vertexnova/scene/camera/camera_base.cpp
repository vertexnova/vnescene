/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "vertexnova/scene/camera/camera_base.h"

namespace vne::scene {

using vne::math::GraphicsApi;
using vne::math::Mat4f;
using vne::math::Quatf;
using vne::math::Vec3f;
using vne::math::Vec4f;

namespace {

constexpr float kEps = 1e-6f;

}  // namespace

Quatf CameraBase::orientationFromPosBack(Vec3f back, Vec3f up_hint, const Vec3f& fallback_up) noexcept {
    float bl = back.length();
    if (bl < kEps) {
        back = Vec3f(0.0f, 0.0f, 1.0f);
    } else {
        back /= bl;
    }

    float ul = up_hint.length();
    if (ul < kEps) {
        up_hint = fallback_up;
        ul = up_hint.length();
    }
    if (ul < kEps) {
        up_hint = Vec3f(0.0f, 1.0f, 0.0f);
    } else {
        up_hint /= ul;
    }

    Vec3f fb = fallback_up;
    float fl = fb.length();
    if (fl < kEps) {
        fb = Vec3f(0.0f, 1.0f, 0.0f);
    } else {
        fb /= fl;
    }

    Vec3f right = up_hint.cross(back);
    float rl = right.length();
    if (rl < kEps) {
        right = fb.cross(back);
        rl = right.length();
    }
    if (rl < kEps) {
        right = Vec3f(1.0f, 0.0f, 0.0f);
    } else {
        right /= rl;
    }
    const Vec3f up = back.cross(right);
    const Mat4f basis_mat(Vec4f(right.x(), right.y(), right.z(), 0.0f),
                          Vec4f(up.x(), up.y(), up.z(), 0.0f),
                          Vec4f(back.x(), back.y(), back.z(), 0.0f),
                          Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    return Quatf(basis_mat).normalized();
}

Mat4f CameraBase::viewFromQuaternion(GraphicsApi api, float s) const noexcept {
    const Vec3f fwd = forwardDirImpl();
    const Vec3f up = upDirImpl();
    return composeViewWithSceneScale(Mat4f::lookAt(position_, position_ + fwd, up, api), s);
}

void CameraBase::setTargetImpl(const Vec3f& target) noexcept {
    Vec3f back = position_ - target;
    const float d = back.length();
    if (d > kEps) {
        look_distance_ = d;
        orientation_ = orientationFromPosBack(back / d, up_hint_, Vec3f(0.0f, 1.0f, 0.0f));
    }
    view_matrix_dirty_ = true;
}

void CameraBase::setUpImpl(const Vec3f& up) noexcept {
    up_hint_ = up;
    Vec3f back = orientation_.getZAxis();
    const float bl = back.length();
    if (bl < kEps) {
        back = Vec3f(0.0f, 0.0f, 1.0f);
    } else {
        back /= bl;
    }
    orientation_ = orientationFromPosBack(back, up_hint_, Vec3f(0.0f, 1.0f, 0.0f));
    view_matrix_dirty_ = true;
}

void CameraBase::setOrientationViewImpl(const Vec3f& pos, const Quatf& q) noexcept {
    position_ = pos;
    orientation_ = q.normalized();
    up_hint_ = orientation_.getYAxis();
    view_matrix_dirty_ = true;
}

void CameraBase::lookAtImpl(const Vec3f& position, const Vec3f& target, const Vec3f& up) noexcept {
    position_ = position;
    up_hint_ = up;
    Vec3f back = position - target;
    const float d = back.length();
    if (d > kEps) {
        look_distance_ = d;
        back /= d;
    } else {
        back = orientation_.getZAxis();
        const float bl = back.length();
        if (bl < kEps) {
            back = Vec3f(0.0f, 0.0f, 1.0f);
        } else {
            back /= bl;
        }
    }
    orientation_ = orientationFromPosBack(back, up_hint_, Vec3f(0.0f, 1.0f, 0.0f));
    view_matrix_dirty_ = true;
}

void CameraBase::lookAtImpl(const Vec3f& target, const Vec3f& up) noexcept {
    up_hint_ = up;
    Vec3f back = position_ - target;
    const float d = back.length();
    if (d > kEps) {
        look_distance_ = d;
        back /= d;
    } else {
        back = orientation_.getZAxis();
        const float bl = back.length();
        if (bl < kEps) {
            back = Vec3f(0.0f, 0.0f, 1.0f);
        } else {
            back /= bl;
        }
    }
    orientation_ = orientationFromPosBack(back, up_hint_, Vec3f(0.0f, 1.0f, 0.0f));
    view_matrix_dirty_ = true;
}

}  // namespace vne::scene
