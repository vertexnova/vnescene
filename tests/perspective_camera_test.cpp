/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   March 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

/**
 * @file perspective_camera_test.cpp
 * @brief Tests for PerspectiveCamera-specific construction, FOV/aspect,
 * movement helpers, resize, clip planes, graphics API, and scene-scale.
 */

#include <gtest/gtest.h>
#include <cmath>
#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTol = 1e-5f;

PerspectiveCamera makePerspective(float fov = 60.0f,
                                  float width = 800.0f,
                                  float height = 600.0f,
                                  float near_plane = 0.1f,
                                  float far_plane = 1000.0f) {
    return PerspectiveCamera(fov, width, height, near_plane, far_plane);
}

}  // namespace

//==============================================================================
// Construction
//==============================================================================

TEST(PerspectiveCameraTest, ConstructFromFovAspectRatio_SetsFovAndAspect) {
    PerspectiveCamera cam(45.0f, 16.0f / 9.0f, 0.1f, 500.0f);
    EXPECT_NEAR(cam.getFieldOfView(), 45.0f, kTol);
    EXPECT_NEAR(cam.getAspectRatio(), 16.0f / 9.0f, kTol);
    EXPECT_NEAR(cam.getNearPlane(), 0.1f, kTol);
    EXPECT_NEAR(cam.getFarPlane(), 500.0f, kTol);
    EXPECT_NEAR(cam.getWidth(), 0.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 0.0f, kTol);
}

TEST(PerspectiveCameraTest, ConstructFromFovWidthHeight_SetsAspectAndViewport) {
    PerspectiveCamera cam(60.0f, 1920.0f, 1080.0f, 0.1f, 1000.0f);
    EXPECT_NEAR(cam.getFieldOfView(), 60.0f, kTol);
    EXPECT_NEAR(cam.getAspectRatio(), 1920.0f / 1080.0f, kTol);
    EXPECT_NEAR(cam.getWidth(), 1920.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 1080.0f, kTol);
}

//==============================================================================
// setPerspective (FOV clamp, aspect minimum, clip planes)
//==============================================================================

TEST(PerspectiveCameraTest, SetPerspective_ClampsFov) {
    PerspectiveCamera cam = makePerspective();
    cam.setPerspective(0.0f, 1.0f, 0.1f, 100.0f);
    EXPECT_GE(cam.getFieldOfView(), 1.0f);
    cam.setPerspective(200.0f, 1.0f, 0.1f, 100.0f);
    EXPECT_LE(cam.getFieldOfView(), 179.0f);
}

TEST(PerspectiveCameraTest, SetPerspective_EnforcesMinAspectRatio) {
    PerspectiveCamera cam = makePerspective();
    cam.setPerspective(60.0f, 0.05f, 0.1f, 100.0f);
    EXPECT_GE(cam.getAspectRatio(), 0.1f);
}

TEST(PerspectiveCameraTest, SetPerspective_UpdatesNearFarPlanes) {
    PerspectiveCamera cam = makePerspective();
    cam.setPerspective(60.0f, 1.0f, 0.5f, 200.0f);
    EXPECT_NEAR(cam.getNearPlane(), 0.5f, kTol);
    EXPECT_NEAR(cam.getFarPlane(), 200.0f, kTol);
}

//==============================================================================
// resize
//==============================================================================

TEST(PerspectiveCameraTest, Resize_UpdatesWidthHeightAndAspectRatio) {
    PerspectiveCamera cam = makePerspective();
    cam.resize(1920.0f, 1080.0f);
    EXPECT_NEAR(cam.getWidth(), 1920.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 1080.0f, kTol);
    EXPECT_NEAR(cam.getAspectRatio(), 1920.0f / 1080.0f, kTol);
}

//==============================================================================
// setNearPlane / setFarPlane / setClipPlanes (min constraints)
//==============================================================================

TEST(PerspectiveCameraTest, SetNearPlane_EnforcesMinimum) {
    PerspectiveCamera cam = makePerspective();
    cam.setNearPlane(0.0001f);
    EXPECT_GE(cam.getNearPlane(), 0.001f);
}

TEST(PerspectiveCameraTest, SetFarPlane_EnforcesOffsetFromNear) {
    PerspectiveCamera cam = makePerspective();
    cam.setNearPlane(1.0f);
    cam.setFarPlane(1.05f);
    EXPECT_GE(cam.getFarPlane(), cam.getNearPlane() + 0.1f);
}

TEST(PerspectiveCameraTest, SetClipPlanes_AppliesBothConstraints) {
    PerspectiveCamera cam = makePerspective();
    cam.setClipPlanes(0.0001f, 0.05f);
    EXPECT_GE(cam.getNearPlane(), 0.001f);
    EXPECT_GE(cam.getFarPlane(), cam.getNearPlane() + 0.1f);
}

//==============================================================================
// setGraphicsApi
//==============================================================================

TEST(PerspectiveCameraTest, SetGraphicsApi_ChangesClipToScreenBehavior) {
    PerspectiveCamera cam_gl = makePerspective();
    PerspectiveCamera cam_metal = makePerspective();
    cam_metal.setGraphicsApi(GraphicsApi::eMetal);
    Mat4f m_gl = cam_gl.getClipToScreenMatrix(800.0f, 600.0f);
    Mat4f m_metal = cam_metal.getClipToScreenMatrix(800.0f, 600.0f);
    EXPECT_NE(cam_gl.getGraphicsApi(), cam_metal.getGraphicsApi());
    bool same = true;
    for (std::size_t col = 0; col < 4 && same; ++col)
        for (std::size_t row = 0; row < 4 && same; ++row)
            same = same && (std::fabs(m_gl[col][row] - m_metal[col][row]) < kTol);
    EXPECT_FALSE(same);
}

//==============================================================================
// getForward / getRight / movement helpers
//==============================================================================

TEST(PerspectiveCameraTest, GetForward_PointsFromPositionToTarget) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Vec3f fwd = cam.getForward();
    EXPECT_NEAR(fwd.x(), 0.0f, kTol);
    EXPECT_NEAR(fwd.y(), 0.0f, kTol);
    EXPECT_NEAR(fwd.z(), -1.0f, kTol);
}

TEST(PerspectiveCameraTest, GetRight_PerpendicularToForwardAndUp) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Vec3f fwd = cam.getForward();
    Vec3f right = cam.getRight();
    float dot = fwd.x() * right.x() + fwd.y() * right.y() + fwd.z() * right.z();
    EXPECT_NEAR(dot, 0.0f, kTol);
}

TEST(PerspectiveCameraTest, MoveForward_TranslatesPositionDerivedTargetFollows) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam.moveForward(2.0f);
    EXPECT_NEAR(cam.getPosition().z(), 3.0f, kTol);
    // Orientation unchanged; look distance preserved → derived target moves with eye.
    EXPECT_NEAR(cam.getTarget().z(), -2.0f, kTol);
}

TEST(PerspectiveCameraTest, MoveRight_TranslatesPositionDerivedTargetFollows) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam.moveRight(1.0f);
    EXPECT_NEAR(cam.getPosition().x(), 1.0f, kTol);
    EXPECT_NEAR(cam.getTarget().x(), 1.0f, kTol);
}

TEST(PerspectiveCameraTest, MoveUp_TranslatesPositionDerivedTargetFollows) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam.moveUp(2.0f);
    EXPECT_NEAR(cam.getPosition().y(), 2.0f, kTol);
    EXPECT_NEAR(cam.getTarget().y(), 2.0f, kTol);
}

//==============================================================================
// Scene-scale (class-specific: view matrix reflects scale)
//==============================================================================

TEST(PerspectiveCameraTest, SetSceneScale_BakesIntoViewMatrix) {
    PerspectiveCamera cam = makePerspective();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Mat4f view_unscaled = cam.getViewMatrix();
    cam.setSceneScale(2.0f);
    Mat4f view_scaled = cam.getViewMatrix();
    // XY-only scale multiplies matrix rows 0 and 1; row 2 (e.g. [3][2]) is unchanged.
    EXPECT_GT(std::fabs(view_scaled[0][0] - view_unscaled[0][0]), kTol);
}

TEST(PerspectiveCameraTest, Projection_IndependentOfSceneScale) {
    PerspectiveCamera cam_a = makePerspective();
    cam_a.setSceneScale(1.0f);
    cam_a.updateProjectionMatrix();
    Mat4f proj_a = cam_a.getProjectionMatrix();

    PerspectiveCamera cam_b = makePerspective();
    cam_b.setSceneScale(4.0f);
    cam_b.updateProjectionMatrix();
    Mat4f proj_b = cam_b.getProjectionMatrix();

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(proj_a[c][r], proj_b[c][r], kTol);
        }
    }
}

TEST(PerspectiveCameraTest, GetForwardDir_MatchesNormalizedTargetMinusPosition) {
    PerspectiveCamera cam = makePerspective();
    cam.lookAt(Vec3f(2.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    cam.updateMatrices();
    Vec3f a = cam.getForwardDir();
    Vec3f b = (cam.getTarget() - cam.getPosition()).normalized();
    EXPECT_NEAR(a.x(), b.x(), kTol);
    EXPECT_NEAR(a.y(), b.y(), kTol);
    EXPECT_NEAR(a.z(), b.z(), kTol);
}

TEST(PerspectiveCameraTest, SetOrientationView_RoundTripOrientation) {
    PerspectiveCamera cam = makePerspective();
    Quatf q = Quatf::fromAxisAngle(Vec3f(0.0f, 1.0f, 0.0f), 0.5f);
    q = q.normalized();
    cam.setOrientationView(Vec3f(1.0f, 2.0f, 3.0f), q);
    const Quatf got = cam.getOrientation().normalized();
    EXPECT_NEAR(std::fabs(got.dot(q)), 1.0f, kTol);
    EXPECT_NEAR(cam.getPosition().x(), 1.0f, kTol);
}

//==============================================================================
// resize edge cases
//==============================================================================

TEST(PerspectiveCameraTest, Resize_ZeroHeight_DoesNotProduceNaN) {
    PerspectiveCamera cam = makePerspective();
    float aspect_before = cam.getAspectRatio();
    cam.resize(800.0f, 0.0f);
    float aspect_after = cam.getAspectRatio();
    EXPECT_EQ(aspect_before, aspect_after) << "aspect ratio must not change when height == 0";
    EXPECT_FALSE(std::isnan(aspect_after));
    EXPECT_FALSE(std::isinf(aspect_after));
}

//==============================================================================
// setSceneScale clamping
//==============================================================================

TEST(PerspectiveCameraTest, SetSceneScale_ZeroIsClamped) {
    PerspectiveCamera cam = makePerspective();
    cam.setSceneScale(0.0f);
    EXPECT_GE(cam.getSceneScale(), 1e-4f);
}

TEST(PerspectiveCameraTest, SetSceneScale_NegativeIsClamped) {
    PerspectiveCamera cam = makePerspective();
    cam.setSceneScale(-5.0f);
    EXPECT_GE(cam.getSceneScale(), 1e-4f);
}

// After calling getViewMatrix() and getProjectionMatrix() individually, the next
// getViewProjectionMatrix() must return P*V using the freshly recomputed matrices,
// not a stale product from before the individual matrix updates.
TEST(PerspectiveCameraTest, ViewProjectionMatrix_ConsistentAfterSeparateGetters) {
    PerspectiveCamera cam(60.0f, 1280.0f, 720.0f, 0.1f, 1000.0f);
    cam.lookAt(Vec3f(3.0f, 2.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    // Trigger individual matrix recomputes without touching VP.
    Mat4f view = cam.getViewMatrix();
    Mat4f proj = cam.getProjectionMatrix();

    // VP must equal P * V (not a stale product from the previous state).
    Mat4f vp = cam.getViewProjectionMatrix();
    Mat4f expected = proj * view;

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(vp[c][r], expected[c][r], kTol)
                << "VP[" << c << "][" << r << "] mismatch after separate getter calls";
        }
    }
}

// toGpu must return a (view, proj, vp) triplet where vp == proj * view.
TEST(PerspectiveCameraTest, ToGpu_ViewProjectionMatchesProductOfViewAndProjection) {
    PerspectiveCamera cam(45.0f, 1920.0f, 1080.0f, 0.1f, 500.0f);
    cam.lookAt(Vec3f(1.0f, 1.0f, 4.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    // Force individual getter paths before toGpu (the S1 regression path).
    [[maybe_unused]] Mat4f v = cam.getViewMatrix();
    [[maybe_unused]] Mat4f p = cam.getProjectionMatrix();

    auto gpu = cam.toGpu();

    auto col4ToMat = [](const Float4& c0, const Float4& c1, const Float4& c2, const Float4& c3) {
        return Mat4f(Vec4f(c0.x, c0.y, c0.z, c0.w),
                     Vec4f(c1.x, c1.y, c1.z, c1.w),
                     Vec4f(c2.x, c2.y, c2.z, c2.w),
                     Vec4f(c3.x, c3.y, c3.z, c3.w));
    };
    Mat4f view_gpu = col4ToMat(gpu.view_col0, gpu.view_col1, gpu.view_col2, gpu.view_col3);
    Mat4f proj_gpu = col4ToMat(gpu.proj_col0, gpu.proj_col1, gpu.proj_col2, gpu.proj_col3);
    Mat4f vp_gpu = col4ToMat(gpu.view_proj_col0, gpu.view_proj_col1, gpu.view_proj_col2, gpu.view_proj_col3);
    Mat4f expected = proj_gpu * view_gpu;

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(vp_gpu[c][r], expected[c][r], kTol)
                << "CameraGpu VP[" << c << "][" << r << "] inconsistent with P*V";
        }
    }
}
