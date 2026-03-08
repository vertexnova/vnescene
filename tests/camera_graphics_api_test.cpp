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

#include <gtest/gtest.h>
#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/orthographic_camera.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTolerance = 1e-5f;

}  // namespace

TEST(CameraGraphicsApi, PerspectiveCamera_DefaultIsOpenGL) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);
}

TEST(CameraGraphicsApi, OrthographicCamera_DefaultIsOpenGL) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);
}

TEST(CameraGraphicsApi, PerspectiveCamera_SetGetRoundTrip) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    cam.setGraphicsApi(GraphicsApi::eVulkan);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eVulkan);

    cam.setGraphicsApi(GraphicsApi::eMetal);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eMetal);

    cam.setGraphicsApi(GraphicsApi::eOpenGL);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);
}

TEST(CameraGraphicsApi, OrthographicCamera_SetGetRoundTrip) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);

    cam.setGraphicsApi(GraphicsApi::eVulkan);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eVulkan);

    cam.setGraphicsApi(GraphicsApi::eDirectX);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eDirectX);

    cam.setGraphicsApi(GraphicsApi::eOpenGL);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);
}

TEST(CameraGraphicsApi, PerspectiveCamera_SetApiMarksMatricesDirty) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cam.setPosition(Vec3f(1.0f, 2.0f, 3.0f));
    cam.updateMatrices();  // Ensure matrices are clean

    // Get projection matrix before API change (OpenGL)
    Mat4f proj_opengl = cam.getProjectionMatrix();
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);

    // Change graphics API to Vulkan
    cam.setGraphicsApi(GraphicsApi::eVulkan);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eVulkan);

    // Matrices should be rebuilt with new API
    Mat4f proj_vulkan = cam.getProjectionMatrix();
    Mat4f vp_after = cam.getViewProjectionMatrix();

    // OpenGL: Y not flipped (proj[1][1] > 0)
    // Vulkan: Y flipped (proj[1][1] < 0)
    EXPECT_GT(proj_opengl[1][1], 0.0f);
    EXPECT_LT(proj_vulkan[1][1], 0.0f);

    // Verify matrices were recalculated (not stale)
    Mat4f vp_recomputed = cam.getViewProjectionMatrix();
    EXPECT_NEAR(vp_after[0][0], vp_recomputed[0][0], kTolerance);
}

TEST(CameraGraphicsApi, OrthographicCamera_SetApiMarksMatricesDirty) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    cam.setPosition(Vec3f(1.0f, 2.0f, 3.0f));
    cam.updateMatrices();  // Ensure matrices are clean

    // Get projection matrix before API change (OpenGL)
    Mat4f proj_opengl = cam.getProjectionMatrix();
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eOpenGL);

    // Change graphics API to Vulkan
    cam.setGraphicsApi(GraphicsApi::eVulkan);
    EXPECT_EQ(cam.getGraphicsApi(), GraphicsApi::eVulkan);

    // Matrices should be rebuilt with new API
    Mat4f proj_vulkan = cam.getProjectionMatrix();
    Mat4f vp_after = cam.getViewProjectionMatrix();

    // OpenGL: Y not flipped (proj[1][1] > 0)
    // Vulkan: Y flipped (proj[1][1] < 0)
    EXPECT_GT(proj_opengl[1][1], 0.0f);
    EXPECT_LT(proj_vulkan[1][1], 0.0f);

    // Verify matrices were recalculated
    Mat4f vp_recomputed = cam.getViewProjectionMatrix();
    EXPECT_NEAR(vp_after[0][0], vp_recomputed[0][0], kTolerance);
}

TEST(CameraGraphicsApi, PerspectiveCamera_OpenGLToVulkan_ProjectionDiffers) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam.setUp(Vec3f(0.0f, 1.0f, 0.0f));

    // Get projection with OpenGL
    cam.setGraphicsApi(GraphicsApi::eOpenGL);
    cam.updateMatrices();
    Mat4f proj_opengl = cam.getProjectionMatrix();

    // Get projection with Vulkan
    cam.setGraphicsApi(GraphicsApi::eVulkan);
    cam.updateMatrices();
    Mat4f proj_vulkan = cam.getProjectionMatrix();

    // OpenGL: Y not flipped (proj[1][1] > 0)
    // Vulkan: Y flipped (proj[1][1] < 0)
    EXPECT_GT(proj_opengl[1][1], 0.0f);
    EXPECT_LT(proj_vulkan[1][1], 0.0f);

    // Verify depth mapping: OpenGL maps near to -1, Vulkan maps near to 0
    float near_z = -cam.getNearPlane();  // Right-handed: negative Z
    Vec4f near_point(0.0f, 0.0f, near_z, 1.0f);
    Vec4f clip_opengl = proj_opengl * near_point;
    Vec4f clip_vulkan = proj_vulkan * near_point;
    float ndc_z_opengl = clip_opengl.z() / clip_opengl.w();
    float ndc_z_vulkan = clip_vulkan.z() / clip_vulkan.w();
    EXPECT_NEAR(ndc_z_opengl, -1.0f, kTolerance);
    EXPECT_NEAR(ndc_z_vulkan, 0.0f, kTolerance);
}

TEST(CameraGraphicsApi, OrthographicCamera_OpenGLToVulkan_ProjectionDiffers) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    cam.setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam.setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam.setUp(Vec3f(0.0f, 1.0f, 0.0f));

    // Get projection with OpenGL
    cam.setGraphicsApi(GraphicsApi::eOpenGL);
    cam.updateMatrices();
    Mat4f proj_opengl = cam.getProjectionMatrix();

    // Get projection with Vulkan
    cam.setGraphicsApi(GraphicsApi::eVulkan);
    cam.updateMatrices();
    Mat4f proj_vulkan = cam.getProjectionMatrix();

    // OpenGL: Y not flipped (proj[1][1] > 0)
    // Vulkan: Y flipped (proj[1][1] < 0)
    EXPECT_GT(proj_opengl[1][1], 0.0f);
    EXPECT_LT(proj_vulkan[1][1], 0.0f);

    // Verify depth mapping: OpenGL maps near to -1, Vulkan maps near to 0
    float near_z = -cam.getNearPlane();  // Right-handed: negative Z
    Vec4f near_point(0.0f, 0.0f, near_z, 1.0f);
    Vec4f clip_opengl = proj_opengl * near_point;
    Vec4f clip_vulkan = proj_vulkan * near_point;
    float ndc_z_opengl = clip_opengl.z() / clip_opengl.w();
    float ndc_z_vulkan = clip_vulkan.z() / clip_vulkan.w();
    EXPECT_NEAR(ndc_z_opengl, -1.0f, kTolerance);
    EXPECT_NEAR(ndc_z_vulkan, 0.0f, kTolerance);
}

TEST(CameraGraphicsApi, PerspectiveCamera_MatricesRemainDirtyUntilAccessed) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cam.updateMatrices();  // Clean state

    // Change API - this should mark matrices dirty
    cam.setGraphicsApi(GraphicsApi::eVulkan);

    // Don't access matrices yet - they should remain dirty
    // Access view matrix - this should trigger rebuild
    Mat4f view = cam.getViewMatrix();

    // Access projection - should also be rebuilt
    Mat4f proj = cam.getProjectionMatrix();

    // Both should now be built with Vulkan API
    // Verify by checking that subsequent calls return the same (cached) values
    Mat4f view2 = cam.getViewMatrix();
    Mat4f proj2 = cam.getProjectionMatrix();

    EXPECT_NEAR(view[0][0], view2[0][0], kTolerance);
    EXPECT_NEAR(proj[0][0], proj2[0][0], kTolerance);
}

TEST(CameraGraphicsApi, OrthographicCamera_MatricesRemainDirtyUntilAccessed) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    cam.updateMatrices();  // Clean state

    // Change API - this should mark matrices dirty
    cam.setGraphicsApi(GraphicsApi::eVulkan);

    // Don't access matrices yet - they should remain dirty
    // Access view matrix - this should trigger rebuild
    Mat4f view = cam.getViewMatrix();

    // Access projection - should also be rebuilt
    Mat4f proj = cam.getProjectionMatrix();

    // Both should now be built with Vulkan API
    Mat4f view2 = cam.getViewMatrix();
    Mat4f proj2 = cam.getProjectionMatrix();

    EXPECT_NEAR(view[0][0], view2[0][0], kTolerance);
    EXPECT_NEAR(proj[0][0], proj2[0][0], kTolerance);
}

TEST(CameraGraphicsApi, PerspectiveCamera_SetSameApiNoOp) {
    PerspectiveCamera cam(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cam.updateMatrices();

    Mat4f proj_before = cam.getProjectionMatrix();

    // Set to same API (OpenGL)
    cam.setGraphicsApi(GraphicsApi::eOpenGL);

    // Matrix should be unchanged (no rebuild needed)
    Mat4f proj_after = cam.getProjectionMatrix();

    EXPECT_NEAR(proj_before[0][0], proj_after[0][0], kTolerance);
    EXPECT_NEAR(proj_before[1][1], proj_after[1][1], kTolerance);
    EXPECT_NEAR(proj_before[2][2], proj_after[2][2], kTolerance);
}

TEST(CameraGraphicsApi, OrthographicCamera_SetSameApiNoOp) {
    OrthographicCamera cam(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 1000.0f);
    cam.updateMatrices();

    Mat4f proj_before = cam.getProjectionMatrix();

    // Set to same API (OpenGL)
    cam.setGraphicsApi(GraphicsApi::eOpenGL);

    // Matrix should be unchanged (no rebuild needed)
    Mat4f proj_after = cam.getProjectionMatrix();

    EXPECT_NEAR(proj_before[0][0], proj_after[0][0], kTolerance);
    EXPECT_NEAR(proj_before[1][1], proj_after[1][1], kTolerance);
    EXPECT_NEAR(proj_before[2][2], proj_after[2][2], kTolerance);
}
