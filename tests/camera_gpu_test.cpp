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
#include "vertexnova/scene/camera/camera_gpu.h"
#include "vertexnova/scene/camera/camera_factory.h"
#include "vertexnova/scene/camera/camera_types.h"
#include <vertexnova/math/core/core.h>
#include <cmath>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTolerance = 1e-5f;

Mat4f float4ColumnsToMat4(const Float4& c0, const Float4& c1, const Float4& c2, const Float4& c3) noexcept {
    return Mat4f(Vec4f(c0.x, c0.y, c0.z, c0.w),
                 Vec4f(c1.x, c1.y, c1.z, c1.w),
                 Vec4f(c2.x, c2.y, c2.z, c2.w),
                 Vec4f(c3.x, c3.y, c3.z, c3.w));
}

}  // namespace

TEST(CameraGpu, Layout_StaticAsserts) {
    EXPECT_EQ(sizeof(CameraGpu), 17u * 16u);
    EXPECT_EQ(alignof(CameraGpu), 16u);
}

TEST(CameraGpu, Packing_ViewProjConsistency) {
    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(1.0f, 2.0f, 5.0f));
    cam->setTarget(Vec3f(0, 0, 0));
    cam->setUp(Vec3f(0, 1, 0));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    CameraGpu gpu = cam->toGpu();

    Mat4f viewProjPacked =
        float4ColumnsToMat4(gpu.view_proj_col0, gpu.view_proj_col1, gpu.view_proj_col2, gpu.view_proj_col3);

    Mat4f viewProjExpected = cam->getViewProjectionMatrix();
    for (int col = 0; col < 4; ++col) {
        const auto& exp = viewProjExpected.getColumn(static_cast<size_t>(col));
        const auto& got = viewProjPacked.getColumn(static_cast<size_t>(col));
        EXPECT_NEAR(exp.x(), got.x(), kTolerance);
        EXPECT_NEAR(exp.y(), got.y(), kTolerance);
        EXPECT_NEAR(exp.z(), got.z(), kTolerance);
        EXPECT_NEAR(exp.w(), got.w(), kTolerance);
    }
}

TEST(CameraGpu, Packing_NearFarMatchStoredClipWhenSceneScale) {
    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setSceneScale(2.0f);
    cam->updateMatrices();

    CameraGpu gpu = cam->toGpu();
    EXPECT_NEAR(gpu.position_near.w, cam->getNearPlane(), kTolerance);
    EXPECT_NEAR(gpu.far_viewport.x, cam->getFarPlane(), kTolerance);
}

TEST(CameraGpu, Packing_DirectionVectorsMatchCamera) {
    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->updateMatrices();

    CameraGpu gpu = cam->toGpu();
    Vec3f f = cam->getForwardDir();
    Vec3f r = cam->getRightDir();
    Vec3f u = cam->getUpDir();
    EXPECT_NEAR(gpu.forward_dir.x, f.x(), kTolerance);
    EXPECT_NEAR(gpu.forward_dir.y, f.y(), kTolerance);
    EXPECT_NEAR(gpu.forward_dir.z, f.z(), kTolerance);
    EXPECT_NEAR(gpu.right_dir.x, r.x(), kTolerance);
    EXPECT_NEAR(gpu.right_dir.y, r.y(), kTolerance);
    EXPECT_NEAR(gpu.right_dir.z, r.z(), kTolerance);
    EXPECT_NEAR(gpu.up_dir.x, u.x(), kTolerance);
    EXPECT_NEAR(gpu.up_dir.y, u.y(), kTolerance);
    EXPECT_NEAR(gpu.up_dir.z, u.z(), kTolerance);
}
