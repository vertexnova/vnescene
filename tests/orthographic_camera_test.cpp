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
    EXPECT_GT(std::fabs(view_scaled[3][2] - view_unscaled[3][2]), kTol);
}
