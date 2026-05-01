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
 * @file orthographic_camera_test.cpp
 * @brief Tests for OrthographicCamera-specific construction, bounds math,
 * resize, aspect ratio, clip planes, graphics API, and scene-scale.
 */

#include <gtest/gtest.h>
#include <cmath>
#include "vertexnova/scene/camera/orthographic_camera.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTol = 1e-5f;

OrthographicCamera makeOrtho(float width = 800.0f,
                             float height = 600.0f,
                             float near_plane = 0.1f,
                             float far_plane = 1000.0f) {
    return OrthographicCamera(width, height, near_plane, far_plane);
}

}  // namespace

//==============================================================================
// Construction
//==============================================================================

TEST(OrthographicCameraTest, ConstructFromExplicitBounds_SetsLeftRightBottomTop) {
    OrthographicCamera cam(-10.0f, 10.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    EXPECT_NEAR(cam.getLeft(), -10.0f, kTol);
    EXPECT_NEAR(cam.getRight(), 10.0f, kTol);
    EXPECT_NEAR(cam.getBottom(), -5.0f, kTol);
    EXPECT_NEAR(cam.getTop(), 5.0f, kTol);
    EXPECT_NEAR(cam.getNearPlane(), 0.1f, kTol);
    EXPECT_NEAR(cam.getFarPlane(), 100.0f, kTol);
    EXPECT_NEAR(cam.getWidth(), 20.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 10.0f, kTol);
}

TEST(OrthographicCameraTest, ConstructFromWidthHeight_CentersBounds) {
    OrthographicCamera cam(400.0f, 300.0f, 0.1f, 1000.0f);
    EXPECT_NEAR(cam.getLeft(), -200.0f, kTol);
    EXPECT_NEAR(cam.getRight(), 200.0f, kTol);
    EXPECT_NEAR(cam.getBottom(), -150.0f, kTol);
    EXPECT_NEAR(cam.getTop(), 150.0f, kTol);
    EXPECT_NEAR(cam.getWidth(), 400.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 300.0f, kTol);
}

//==============================================================================
// resize (centered bounds)
//==============================================================================

TEST(OrthographicCameraTest, Resize_CentersBoundsAndUpdatesWidthHeight) {
    OrthographicCamera cam = makeOrtho();
    cam.resize(400.0f, 300.0f);
    EXPECT_NEAR(cam.getLeft(), -200.0f, kTol);
    EXPECT_NEAR(cam.getRight(), 200.0f, kTol);
    EXPECT_NEAR(cam.getBottom(), -150.0f, kTol);
    EXPECT_NEAR(cam.getTop(), 150.0f, kTol);
    EXPECT_NEAR(cam.getWidth(), 400.0f, kTol);
    EXPECT_NEAR(cam.getHeight(), 300.0f, kTol);
}

//==============================================================================
// setBounds
//==============================================================================

TEST(OrthographicCameraTest, SetBounds_UpdatesAllFrustumPlanes) {
    OrthographicCamera cam = makeOrtho();
    cam.setBounds(-20.0f, 20.0f, -10.0f, 10.0f, 0.5f, 500.0f);
    EXPECT_NEAR(cam.getLeft(), -20.0f, kTol);
    EXPECT_NEAR(cam.getRight(), 20.0f, kTol);
    EXPECT_NEAR(cam.getBottom(), -10.0f, kTol);
    EXPECT_NEAR(cam.getTop(), 10.0f, kTol);
    EXPECT_NEAR(cam.getNearPlane(), 0.5f, kTol);
    EXPECT_NEAR(cam.getFarPlane(), 500.0f, kTol);
}

//==============================================================================
// setNearPlane / setFarPlane / setClipPlanes (min constraints)
//==============================================================================

TEST(OrthographicCameraTest, SetNearPlane_EnforcesMinimum) {
    OrthographicCamera cam = makeOrtho();
    cam.setNearPlane(0.0001f);
    EXPECT_GE(cam.getNearPlane(), 0.001f);
}

TEST(OrthographicCameraTest, SetFarPlane_EnforcesOffsetFromNear) {
    OrthographicCamera cam = makeOrtho();
    cam.setNearPlane(1.0f);
    cam.setFarPlane(1.05f);
    EXPECT_GE(cam.getFarPlane(), cam.getNearPlane() + 0.1f);
}

TEST(OrthographicCameraTest, SetClipPlanes_AppliesBothConstraints) {
    OrthographicCamera cam = makeOrtho();
    cam.setClipPlanes(0.0001f, 0.05f);
    EXPECT_GE(cam.getNearPlane(), 0.001f);
    EXPECT_GE(cam.getFarPlane(), cam.getNearPlane() + 0.1f);
}

//==============================================================================
// getAspectRatio
//==============================================================================

TEST(OrthographicCameraTest, GetAspectRatio_EqualsWidthOverHeight) {
    OrthographicCamera cam(800.0f, 600.0f, 0.1f, 1000.0f);
    EXPECT_NEAR(cam.getAspectRatio(), 800.0f / 600.0f, kTol);
}

TEST(OrthographicCameraTest, GetAspectRatio_UpdatesAfterResize) {
    OrthographicCamera cam = makeOrtho();
    cam.resize(1920.0f, 1080.0f);
    EXPECT_NEAR(cam.getAspectRatio(), 1920.0f / 1080.0f, kTol);
}

//==============================================================================
// setGraphicsApi
//==============================================================================

TEST(OrthographicCameraTest, SetGraphicsApi_ChangesClipToScreenBehavior) {
    OrthographicCamera cam_gl = makeOrtho();
    OrthographicCamera cam_metal = makeOrtho();
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
// Scene-scale (baked into view matrix)
//==============================================================================

TEST(OrthographicCameraTest, SetSceneScale_BakesIntoViewMatrix) {
    OrthographicCamera cam = makeOrtho();
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Mat4f view_unscaled = cam.getViewMatrix();
    cam.setSceneScale(3.0f);
    Mat4f view_scaled = cam.getViewMatrix();
    EXPECT_GT(std::fabs(view_scaled[0][0] - view_unscaled[0][0]), kTol);
}

TEST(OrthographicCameraTest, Projection_IndependentOfSceneScale) {
    OrthographicCamera cam_a = makeOrtho();
    cam_a.setSceneScale(1.0f);
    cam_a.updateProjectionMatrix();
    Mat4f proj_a = cam_a.getProjectionMatrix();

    OrthographicCamera cam_b = makeOrtho();
    cam_b.setSceneScale(3.0f);
    cam_b.updateProjectionMatrix();
    Mat4f proj_b = cam_b.getProjectionMatrix();

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(proj_a[c][r], proj_b[c][r], kTol);
        }
    }
}

TEST(OrthographicCameraTest, GetForwardDir_MatchesNormalizedTargetMinusPosition) {
    OrthographicCamera cam = makeOrtho();
    cam.lookAt(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    cam.updateMatrices();
    Vec3f a = cam.getForwardDir();
    Vec3f b = (cam.getTarget() - cam.getPosition()).normalized();
    EXPECT_NEAR(a.x(), b.x(), kTol);
    EXPECT_NEAR(a.y(), b.y(), kTol);
    EXPECT_NEAR(a.z(), b.z(), kTol);
}

TEST(OrthographicCameraTest, SetOrientationView_RoundTripOrientation) {
    OrthographicCamera cam = makeOrtho();
    Quatf q = Quatf::fromAxisAngle(Vec3f(1.0f, 0.0f, 0.0f), 0.3f);
    q = q.normalized();
    cam.setOrientationView(Vec3f(0.0f, 5.0f, 10.0f), q);
    const Quatf got = cam.getOrientation().normalized();
    EXPECT_NEAR(std::fabs(got.dot(q)), 1.0f, kTol);
}

//==============================================================================
// setBounds edge cases
//==============================================================================

TEST(OrthographicCameraTest, SetBounds_ClampsNearAndFarLikeSetters) {
    OrthographicCamera cam = makeOrtho();
    // near < kMinNearPlane and far < near + kMinFarPlaneOffset
    cam.setBounds(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 0.0f);
    EXPECT_GE(cam.getNearPlane(), 0.001f);
    EXPECT_GE(cam.getFarPlane(), cam.getNearPlane() + 0.1f);
}

TEST(OrthographicCameraTest, SetBounds_NearEqualsFar_FarIsAdjusted) {
    OrthographicCamera cam = makeOrtho();
    cam.setBounds(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 1.0f);
    EXPECT_GT(cam.getFarPlane(), cam.getNearPlane());
}

//==============================================================================
// setSceneScale clamping
//==============================================================================

TEST(OrthographicCameraTest, SetSceneScale_ZeroIsClamped) {
    OrthographicCamera cam = makeOrtho();
    cam.setSceneScale(0.0f);
    EXPECT_GE(cam.getSceneScale(), 1e-4f);
}

TEST(OrthographicCameraTest, SetSceneScale_NegativeIsClamped) {
    OrthographicCamera cam = makeOrtho();
    cam.setSceneScale(-2.0f);
    EXPECT_GE(cam.getSceneScale(), 1e-4f);
}

TEST(OrthographicCameraTest, ViewProjectionMatrix_ConsistentAfterSeparateGetters) {
    OrthographicCamera cam(-400.0f, 400.0f, -300.0f, 300.0f, 0.1f, 1000.0f);
    cam.lookAt(Vec3f(0.0f, 5.0f, 10.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    Mat4f view = cam.getViewMatrix();
    Mat4f proj = cam.getProjectionMatrix();
    Mat4f vp = cam.getViewProjectionMatrix();
    Mat4f expected = proj * view;

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(vp[c][r], expected[c][r], kTol)
                << "VP[" << c << "][" << r << "] mismatch after separate getter calls";
        }
    }
}

TEST(OrthographicCameraTest, ToGpu_ViewProjectionMatchesProductOfViewAndProjection) {
    OrthographicCamera cam(800.0f, 600.0f, 0.1f, 100.0f);
    cam.lookAt(Vec3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

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
    Mat4f vp_gpu   = col4ToMat(gpu.view_proj_col0, gpu.view_proj_col1, gpu.view_proj_col2, gpu.view_proj_col3);
    Mat4f expected = proj_gpu * view_gpu;

    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_NEAR(vp_gpu[c][r], expected[c][r], kTol)
                << "CameraGpu VP[" << c << "][" << r << "] inconsistent with P*V";
        }
    }
}

TEST(OrthographicCameraTest, ConstructExplicit_InvertedLeftRight_IsFixedUp) {
    // Passing left > right must not produce a degenerate matrix.
    OrthographicCamera cam(10.0f, -10.0f, -5.0f, 5.0f, 0.1f, 100.0f);
    EXPECT_LT(cam.getLeft(), cam.getRight());
}

TEST(OrthographicCameraTest, ConstructExplicit_InvertedBottomTop_IsFixedUp) {
    OrthographicCamera cam(-10.0f, 10.0f, 5.0f, -5.0f, 0.1f, 100.0f);
    EXPECT_LT(cam.getBottom(), cam.getTop());
}

TEST(OrthographicCameraTest, SetBounds_InvertedHorizontal_IsFixedUp) {
    OrthographicCamera cam = makeOrtho();
    cam.setBounds(5.0f, -5.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    EXPECT_LT(cam.getLeft(), cam.getRight());
}

TEST(OrthographicCameraTest, SetBounds_InvertedVertical_IsFixedUp) {
    OrthographicCamera cam = makeOrtho();
    cam.setBounds(-5.0f, 5.0f, 10.0f, -10.0f, 0.1f, 100.0f);
    EXPECT_LT(cam.getBottom(), cam.getTop());
}

TEST(OrthographicCameraTest, Resize_ZeroWidth_DoesNotProduceDegenerateMatrix) {
    OrthographicCamera cam = makeOrtho();
    cam.resize(0.0f, 600.0f);
    EXPECT_GT(cam.getRight() - cam.getLeft(), 0.0f);
    Mat4f proj = cam.getProjectionMatrix();
    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_FALSE(std::isnan(proj[c][r])) << "NaN in projection[" << c << "][" << r << "]";
            EXPECT_FALSE(std::isinf(proj[c][r])) << "Inf in projection[" << c << "][" << r << "]";
        }
    }
}

TEST(OrthographicCameraTest, Resize_ZeroHeight_DoesNotProduceDegenerateMatrix) {
    OrthographicCamera cam = makeOrtho();
    cam.resize(800.0f, 0.0f);
    EXPECT_GT(cam.getTop() - cam.getBottom(), 0.0f);
    Mat4f proj = cam.getProjectionMatrix();
    for (std::size_t c = 0; c < 4; ++c) {
        for (std::size_t r = 0; r < 4; ++r) {
            EXPECT_FALSE(std::isnan(proj[c][r])) << "NaN in projection[" << c << "][" << r << "]";
            EXPECT_FALSE(std::isinf(proj[c][r])) << "Inf in projection[" << c << "][" << r << "]";
        }
    }
}

TEST(OrthographicCameraTest, Resize_NegativeDimensions_DoesNotProduceDegenerateMatrix) {
    OrthographicCamera cam = makeOrtho();
    cam.resize(-800.0f, -600.0f);
    EXPECT_GT(cam.getRight() - cam.getLeft(), 0.0f);
    EXPECT_GT(cam.getTop() - cam.getBottom(), 0.0f);
}
