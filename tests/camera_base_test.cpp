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
 * @file camera_base_test.cpp
 * @brief Tests for CameraBase shared state and lookAtImpl helpers.
 *
 * Uses a test harness that publicly inherits CameraBase to expose
 * protected members for verification.
 */

#include <gtest/gtest.h>
#include "vertexnova/scene/camera/camera_base.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTol = 1e-5f;

/**
 * Test-only harness that publicly inherits CameraBase to expose protected
 * state and lookAtImpl for testing.
 */
class TestCameraBaseHarness : public CameraBase {
   public:
    const Vec3f& getPosition() const { return position_; }
    const Vec3f& getTarget() const { return target_; }
    const Vec3f& getUp() const { return up_; }
    const std::string& getName() const { return name_; }
    bool getActive() const { return active_; }
    GraphicsApi getGraphicsApi() const { return graphics_api_; }
    float getSceneScale() const { return scene_scale_; }
    bool isViewMatrixDirty() const { return view_matrix_dirty_; }
    bool isProjectionMatrixDirty() const { return projection_matrix_dirty_; }

    void callLookAtImpl(const Vec3f& position, const Vec3f& target, const Vec3f& up) noexcept {
        lookAtImpl(position, target, up);
    }
    void callLookAtImpl(const Vec3f& target, const Vec3f& up) noexcept { lookAtImpl(target, up); }
};

}  // namespace

//==============================================================================
// Default-initialized state
//==============================================================================

TEST(CameraBaseTest, DefaultConstruct_InitializesPositionToOrigin) {
    TestCameraBaseHarness h;
    EXPECT_NEAR(h.getPosition().x(), 0.0f, kTol);
    EXPECT_NEAR(h.getPosition().y(), 0.0f, kTol);
    EXPECT_NEAR(h.getPosition().z(), 0.0f, kTol);
}

TEST(CameraBaseTest, DefaultConstruct_InitializesTargetToNegativeZ) {
    TestCameraBaseHarness h;
    EXPECT_NEAR(h.getTarget().x(), 0.0f, kTol);
    EXPECT_NEAR(h.getTarget().y(), 0.0f, kTol);
    EXPECT_NEAR(h.getTarget().z(), -1.0f, kTol);
}

TEST(CameraBaseTest, DefaultConstruct_InitializesUpToPositiveY) {
    TestCameraBaseHarness h;
    EXPECT_NEAR(h.getUp().x(), 0.0f, kTol);
    EXPECT_NEAR(h.getUp().y(), 1.0f, kTol);
    EXPECT_NEAR(h.getUp().z(), 0.0f, kTol);
}

TEST(CameraBaseTest, DefaultConstruct_NameEmpty) {
    TestCameraBaseHarness h;
    EXPECT_TRUE(h.getName().empty());
}

TEST(CameraBaseTest, DefaultConstruct_ActiveTrue) {
    TestCameraBaseHarness h;
    EXPECT_TRUE(h.getActive());
}

TEST(CameraBaseTest, DefaultConstruct_GraphicsApiOpenGL) {
    TestCameraBaseHarness h;
    EXPECT_EQ(h.getGraphicsApi(), GraphicsApi::eOpenGL);
}

TEST(CameraBaseTest, DefaultConstruct_SceneScaleOne) {
    TestCameraBaseHarness h;
    EXPECT_NEAR(h.getSceneScale(), 1.0f, kTol);
}

TEST(CameraBaseTest, DefaultConstruct_ViewAndProjectionMatrixDirty) {
    TestCameraBaseHarness h;
    EXPECT_TRUE(h.isViewMatrixDirty());
    EXPECT_TRUE(h.isProjectionMatrixDirty());
}

//==============================================================================
// lookAtImpl(position, target, up)
//==============================================================================

TEST(CameraBaseTest, LookAtImpl_ThreeArg_SetsPositionTargetUp) {
    TestCameraBaseHarness h;
    Vec3f pos(1.0f, 2.0f, 3.0f);
    Vec3f target(0.0f, 0.0f, 0.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);

    h.callLookAtImpl(pos, target, up);

    EXPECT_NEAR(h.getPosition().x(), pos.x(), kTol);
    EXPECT_NEAR(h.getPosition().y(), pos.y(), kTol);
    EXPECT_NEAR(h.getPosition().z(), pos.z(), kTol);
    EXPECT_NEAR(h.getTarget().x(), target.x(), kTol);
    EXPECT_NEAR(h.getTarget().y(), target.y(), kTol);
    EXPECT_NEAR(h.getTarget().z(), target.z(), kTol);
    EXPECT_NEAR(h.getUp().x(), up.x(), kTol);
    EXPECT_NEAR(h.getUp().y(), up.y(), kTol);
    EXPECT_NEAR(h.getUp().z(), up.z(), kTol);
}

TEST(CameraBaseTest, LookAtImpl_ThreeArg_MarksViewMatrixDirty) {
    TestCameraBaseHarness h;
    h.callLookAtImpl(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    EXPECT_TRUE(h.isViewMatrixDirty());
}

//==============================================================================
// lookAtImpl(target, up) — keeps position
//==============================================================================

TEST(CameraBaseTest, LookAtImpl_TwoArg_KeepsPositionUnchanged) {
    TestCameraBaseHarness h;
    h.callLookAtImpl(Vec3f(5.0f, 5.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    EXPECT_NEAR(h.getPosition().x(), 5.0f, kTol);
    EXPECT_NEAR(h.getPosition().y(), 5.0f, kTol);
    EXPECT_NEAR(h.getPosition().z(), 5.0f, kTol);
}

TEST(CameraBaseTest, LookAtImpl_TwoArg_UpdatesTargetAndUp) {
    TestCameraBaseHarness h;
    h.callLookAtImpl(Vec3f(1.0f, 0.0f, 0.0f), Vec3f(2.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));

    Vec3f target(2.0f, 0.0f, 0.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);
    EXPECT_NEAR(h.getTarget().x(), target.x(), kTol);
    EXPECT_NEAR(h.getTarget().y(), target.y(), kTol);
    EXPECT_NEAR(h.getTarget().z(), target.z(), kTol);
    EXPECT_NEAR(h.getUp().x(), up.x(), kTol);
    EXPECT_NEAR(h.getUp().y(), up.y(), kTol);
    EXPECT_NEAR(h.getUp().z(), up.z(), kTol);
}

TEST(CameraBaseTest, LookAtImpl_TwoArg_MarksViewMatrixDirty) {
    TestCameraBaseHarness h;
    h.callLookAtImpl(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    EXPECT_TRUE(h.isViewMatrixDirty());
}
