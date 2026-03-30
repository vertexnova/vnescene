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
 * @file camera_test.cpp
 * @brief Tests for ICamera interface methods promoted in Phase 2.
 *
 * All tests operate via ICamera* to verify that the promoted API works
 * without downcasting. Covers: lookAt, setNearPlane/setFarPlane/setClipPlanes,
 * resize, setSceneScale/getSceneScale, getClipToScreenMatrix.
 */

#include <gtest/gtest.h>
#include <cmath>
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/orthographic_camera.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/projection_utils.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTol = 1e-5f;

// Helper: build camera instances for testing (used via ICamera&).
PerspectiveCamera makePerspective() {
    return PerspectiveCamera(60.0f, 800.0f, 600.0f, 0.1f, 1000.0f);
}

OrthographicCamera makeOrtho() {
    return OrthographicCamera(800.0f, 600.0f, 0.1f, 1000.0f);
}

}  // namespace

TEST(CameraTest, GetCameraType_Perspective_ReturnsPerspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;
    EXPECT_EQ(c.getCameraType(), CameraType::ePerspective);
}

TEST(CameraTest, GetCameraType_Orthographic_ReturnsOrthographic) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;
    EXPECT_EQ(c.getCameraType(), CameraType::eOrthographic);
}

//==============================================================================
// lookAt — three-argument form
//==============================================================================

TEST(CameraTest, LookAt_ThreeArg_Perspective_SetsAllPose) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    Vec3f pos(1.0f, 2.0f, 3.0f);
    Vec3f target(0.0f, 0.0f, 0.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);
    c.lookAt(pos, target, up);

    EXPECT_NEAR(c.getPosition().x(), pos.x(), kTol);
    EXPECT_NEAR(c.getPosition().y(), pos.y(), kTol);
    EXPECT_NEAR(c.getPosition().z(), pos.z(), kTol);
    EXPECT_NEAR(c.getTarget().x(), target.x(), kTol);
    EXPECT_NEAR(c.getUp().y(), 1.0f, kTol);

    // View matrix must reflect the new pose.
    Mat4f view_individual = Mat4f::lookAt(pos, target, up, GraphicsApi::eOpenGL);
    Mat4f view_lookat = c.getViewMatrix();
    EXPECT_NEAR(view_individual[3][0], view_lookat[3][0], kTol);
    EXPECT_NEAR(view_individual[3][1], view_lookat[3][1], kTol);
    EXPECT_NEAR(view_individual[3][2], view_lookat[3][2], kTol);
}

TEST(CameraTest, LookAt_ThreeArg_Ortho_SetsAllPose) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    Vec3f pos(5.0f, 5.0f, 5.0f);
    Vec3f target(0.0f, 0.0f, 0.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);
    c.lookAt(pos, target, up);

    EXPECT_NEAR(c.getPosition().x(), 5.0f, kTol);
    EXPECT_NEAR(c.getPosition().y(), 5.0f, kTol);
    EXPECT_NEAR(c.getPosition().z(), 5.0f, kTol);
    EXPECT_NEAR(c.getTarget().x(), 0.0f, kTol);
}

//==============================================================================
// lookAt — two-argument form (keeps position)
//==============================================================================

TEST(CameraTest, LookAt_TwoArg_Perspective_KeepsPosition) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    // Establish a known position first.
    c.setPosition(Vec3f(3.0f, 4.0f, 5.0f));
    c.lookAt(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    // Position must be unchanged.
    EXPECT_NEAR(c.getPosition().x(), 3.0f, kTol);
    EXPECT_NEAR(c.getPosition().y(), 4.0f, kTol);
    EXPECT_NEAR(c.getPosition().z(), 5.0f, kTol);
    // Target must be updated.
    EXPECT_NEAR(c.getTarget().x(), 0.0f, kTol);
    EXPECT_NEAR(c.getTarget().y(), 0.0f, kTol);
    EXPECT_NEAR(c.getTarget().z(), 0.0f, kTol);
}

TEST(CameraTest, LookAt_TwoArg_Ortho_KeepsPosition) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    c.setPosition(Vec3f(1.0f, 2.0f, 3.0f));
    c.lookAt(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    EXPECT_NEAR(c.getPosition().x(), 1.0f, kTol);
    EXPECT_NEAR(c.getPosition().y(), 2.0f, kTol);
    EXPECT_NEAR(c.getPosition().z(), 3.0f, kTol);
    EXPECT_NEAR(c.getTarget().z(), 0.0f, kTol);
}

//==============================================================================
// setNearPlane / setFarPlane / setClipPlanes
//==============================================================================

TEST(CameraTest, SetNearFar_ViaInterface_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    c.setNearPlane(0.5f);
    EXPECT_NEAR(c.getNearPlane(), 0.5f, kTol);

    c.setFarPlane(500.0f);
    EXPECT_NEAR(c.getFarPlane(), 500.0f, kTol);
}

TEST(CameraTest, SetNearFar_ViaInterface_Ortho) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    c.setNearPlane(1.0f);
    EXPECT_NEAR(c.getNearPlane(), 1.0f, kTol);

    c.setFarPlane(200.0f);
    EXPECT_NEAR(c.getFarPlane(), 200.0f, kTol);
}

TEST(CameraTest, SetClipPlanes_ViaInterface_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    c.setClipPlanes(0.2f, 800.0f);
    EXPECT_NEAR(c.getNearPlane(), 0.2f, kTol);
    EXPECT_NEAR(c.getFarPlane(), 800.0f, kTol);
}

TEST(CameraTest, SetClipPlanes_ViaInterface_Ortho) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    c.setClipPlanes(0.5f, 300.0f);
    EXPECT_NEAR(c.getNearPlane(), 0.5f, kTol);
    EXPECT_NEAR(c.getFarPlane(), 300.0f, kTol);
}

//==============================================================================
// resize via ICamera interface
//==============================================================================

TEST(CameraTest, Resize_ViaInterface_Perspective_UpdatesViewportSize) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    c.resize(1920.0f, 1080.0f);

    EXPECT_NEAR(c.getWidth(), 1920.0f, kTol);
    EXPECT_NEAR(c.getHeight(), 1080.0f, kTol);
}

TEST(CameraTest, Resize_ViaInterface_Ortho_UpdatesFrustumSize) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    c.resize(400.0f, 300.0f);

    EXPECT_NEAR(c.getWidth(), 400.0f, kTol);
    EXPECT_NEAR(c.getHeight(), 300.0f, kTol);
}

//==============================================================================
// setSceneScale / getSceneScale
//==============================================================================

TEST(CameraTest, SceneScale_DefaultIsOne_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;
    EXPECT_NEAR(c.getSceneScale(), 1.0f, kTol);
}

TEST(CameraTest, SceneScale_DefaultIsOne_Ortho) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;
    EXPECT_NEAR(c.getSceneScale(), 1.0f, kTol);
}

TEST(CameraTest, SceneScale_BakedIntoViewMatrix_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    c.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    c.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Mat4f view_unscaled = c.getViewMatrix();

    c.setSceneScale(2.0f);
    EXPECT_NEAR(c.getSceneScale(), 2.0f, kTol);

    Mat4f view_scaled = c.getViewMatrix();
    // Scene scale is XY-only (rows 0 and 1 of V are scaled); row 2 is unchanged.
    EXPECT_GT(std::fabs(view_scaled[0][0] - view_unscaled[0][0]), kTol);
}

TEST(CameraTest, SceneScale_BakedIntoViewMatrix_Ortho) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    c.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    c.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Mat4f view_unscaled = c.getViewMatrix();

    c.setSceneScale(3.0f);
    EXPECT_NEAR(c.getSceneScale(), 3.0f, kTol);

    Mat4f view_scaled = c.getViewMatrix();
    EXPECT_GT(std::fabs(view_scaled[0][0] - view_unscaled[0][0]), kTol);
}

TEST(CameraTest, SceneScale_ResetToOne_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    c.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    c.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    Mat4f view_original = c.getViewMatrix();

    c.setSceneScale(2.0f);
    c.setSceneScale(1.0f);
    Mat4f view_reset = c.getViewMatrix();

    // Should match the original (no scale).
    for (std::size_t col = 0; col < 4; ++col) {
        for (std::size_t row = 0; row < 4; ++row) {
            EXPECT_NEAR(view_original[col][row], view_reset[col][row], kTol);
        }
    }
}

//==============================================================================
// getClipToScreenMatrix
//==============================================================================

TEST(CameraTest, ClipToScreenMatrix_CenterNDC_Perspective) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    Mat4f m = c.getClipToScreenMatrix(800.0f, 600.0f);
    Vec4f center_ndc(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4f screen = m * center_ndc;

    EXPECT_NEAR(screen.x(), 400.0f, kTol);
    EXPECT_NEAR(screen.y(), 300.0f, kTol);
}

TEST(CameraTest, ClipToScreenMatrix_CenterNDC_Ortho) {
    OrthographicCamera cam = makeOrtho();
    ICamera& c = cam;

    Mat4f m = c.getClipToScreenMatrix(800.0f, 600.0f);
    Vec4f center_ndc(0.0f, 0.0f, 0.0f, 1.0f);
    Vec4f screen = m * center_ndc;

    EXPECT_NEAR(screen.x(), 400.0f, kTol);
    EXPECT_NEAR(screen.y(), 300.0f, kTol);
}

TEST(CameraTest, ClipToScreenMatrix_MatchesFreeFunctionResult) {
    PerspectiveCamera cam = makePerspective();
    ICamera& c = cam;

    Mat4f from_interface = c.getClipToScreenMatrix(800.0f, 600.0f);
    Mat4f from_util = vne::math::clipToScreenMatrix(800.0f, 600.0f, GraphicsApi::eOpenGL);

    for (std::size_t col = 0; col < 4; ++col) {
        for (std::size_t row = 0; row < 4; ++row) {
            EXPECT_NEAR(from_interface[col][row], from_util[col][row], kTol);
        }
    }
}

TEST(CameraTest, ClipToScreenMatrix_ApiDifference_MetalYFlip) {
    PerspectiveCamera cam_gl = makePerspective();
    PerspectiveCamera cam_metal = makePerspective();
    cam_metal.setGraphicsApi(GraphicsApi::eMetal);

    // NDC top (0, 1):
    //   OpenGL (bottom-left, no proj Y-flip): scale_y = +300 → screen_y = 300 + 300 = 600
    //   Metal  (top-left, no proj Y-flip):    scale_y = -300 → screen_y = 300 - 300 = 0
    Vec4f top_ndc(0.0f, 1.0f, 0.0f, 1.0f);
    Vec4f screen_gl = cam_gl.getClipToScreenMatrix(800.0f, 600.0f) * top_ndc;
    Vec4f screen_metal = cam_metal.getClipToScreenMatrix(800.0f, 600.0f) * top_ndc;

    EXPECT_GT(screen_gl.y(), screen_metal.y());
}
