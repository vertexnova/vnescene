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
#include "vertexnova/scene/camera/camera_utils.h"
#include "vertexnova/scene/camera/camera_factory.h"
#include "vertexnova/scene/camera/camera_types.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/geometry/frustum.h>
#include <vertexnova/math/geometry/aabb.h>
#include <vertexnova/math/projection_utils.h>
#include <vertexnova/math/viewport.h>
#include <cmath>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTolerance = 1e-5f;

std::shared_ptr<PerspectiveCamera> makeCenterCamera() {
    PerspectiveCameraParameters params(60.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, -1.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();
    return cam;
}

}  // namespace

TEST(CameraUtils, ScreenToWorldRay_CenterPointsForward) {
    auto cam = makeCenterCamera();
    float w = 800.0f, h = 600.0f;
    Ray ray = screenToWorldRay(*cam, 400.0f, 300.0f, w, h, GraphicsApi::eOpenGL);
    Vec3f forward = (cam->getTarget() - cam->getPosition()).normalized();
    float dot =
        ray.direction().x() * forward.x() + ray.direction().y() * forward.y() + ray.direction().z() * forward.z();
    EXPECT_NEAR(dot, 1.0f, kTolerance);
}

TEST(CameraUtils, ScreenToWorldRay_CornersDiffer) {
    auto cam = makeCenterCamera();
    float w = 800.0f, h = 600.0f;
    Ray r00 = screenToWorldRay(*cam, 0.0f, 0.0f, w, h);
    Ray r80 = screenToWorldRay(*cam, 800.0f, 0.0f, w, h);
    Ray r06 = screenToWorldRay(*cam, 0.0f, 600.0f, w, h);
    Ray r86 = screenToWorldRay(*cam, 800.0f, 600.0f, w, h);
    EXPECT_FALSE(r00.direction().areSame(r80.direction(), kTolerance));
    EXPECT_FALSE(r00.direction().areSame(r06.direction(), kTolerance));
    EXPECT_FALSE(r80.direction().areSame(r86.direction(), kTolerance));
}

TEST(CameraUtils, ScreenToWorldRay_OriginEqualsCameraPosition) {
    auto cam = makeCenterCamera();
    cam->setPosition(Vec3f(1.0f, 2.0f, 3.0f));
    cam->updateMatrices();
    Ray ray = screenToWorldRay(*cam, 400.0f, 300.0f, 800.0f, 600.0f);
    EXPECT_NEAR(ray.origin().x(), 1.0f, kTolerance);
    EXPECT_NEAR(ray.origin().y(), 2.0f, kTolerance);
    EXPECT_NEAR(ray.origin().z(), 3.0f, kTolerance);
}

TEST(CameraUtils, ScreenToWorldRay_Symmetry) {
    auto cam = makeCenterCamera();
    float w = 800.0f, h = 600.0f;
    Ray left = screenToWorldRay(*cam, 200.0f, 300.0f, w, h);
    Ray right = screenToWorldRay(*cam, 600.0f, 300.0f, w, h);
    EXPECT_NEAR(left.direction().x(), -right.direction().x(), kTolerance);
    EXPECT_NEAR(left.direction().y(), right.direction().y(), kTolerance);
    EXPECT_NEAR(left.direction().z(), right.direction().z(), kTolerance);
}

TEST(CameraUtils, Frustum_ContainsAABBInFront) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    Frustum f = buildFrustum(*cam);
    Aabb aabb = Aabb::fromCenterAndHalfExtents(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_TRUE(f.intersects(aabb));
}

TEST(CameraUtils, Frustum_RejectsAABBBehindCamera) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    Frustum f = buildFrustum(*cam);
    Aabb aabb = Aabb::fromCenterAndHalfExtents(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(f.intersects(aabb));
}

TEST(CameraUtils, Frustum_RejectsBeyondFarPlane) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, -50.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    Frustum f = buildFrustum(*cam);
    Aabb aabb = Aabb::fromCenterAndHalfExtents(Vec3f(0.0f, 0.0f, 200.0f), Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(f.intersects(aabb));
}

TEST(CameraUtils, FitToAABB_ProjectsInsideNDC) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 10.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    Aabb aabb = Aabb::fromCenterAndHalfExtents(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(2.0f, 2.0f, 2.0f));
    fitToAabb(*cam, aabb, 1.1f);

    const float viewport_w = 800.0f;
    const float viewport_h = 600.0f;
    const float ndc_tolerance = 0.02f;  // Allow small margin for floating point
    const float ndc_min = -1.0f - ndc_tolerance;
    const float ndc_max = 1.0f + ndc_tolerance;

    // Project all 8 AABB corners and verify each falls within NDC [-1, 1] x [-1, 1]
    for (uint32_t i = 0; i < 8u; ++i) {
        Vec3f corner = aabb.corner(i);
        Vec3f screen = project(*cam, corner, viewport_w, viewport_h);
        float ndc_x = (screen.x() / viewport_w) * 2.0f - 1.0f;
        float ndc_y = (screen.y() / viewport_h) * 2.0f - 1.0f;
        EXPECT_GE(ndc_x, ndc_min) << "Corner " << i << " NDC x " << ndc_x << " below -1";
        EXPECT_LE(ndc_x, ndc_max) << "Corner " << i << " NDC x " << ndc_x << " above 1";
        EXPECT_GE(ndc_y, ndc_min) << "Corner " << i << " NDC y " << ndc_y << " below -1";
        EXPECT_LE(ndc_y, ndc_max) << "Corner " << i << " NDC y " << ndc_y << " above 1";
    }
}

TEST(CameraUtils, FitToAABB_MonotonicDistance) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 1000.0f);
    auto cam1 = CameraFactory::createPerspective(params);
    cam1->setPosition(Vec3f(0.0f, 0.0f, 20.0f));
    cam1->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam1->resize(800.0f, 600.0f);
    cam1->updateMatrices();

    Aabb small = Aabb::fromCenterAndHalfExtents(Vec3f(0, 0, 0), Vec3f(1.0f, 1.0f, 1.0f));
    fitToAabb(*cam1, small, 1.1f);
    float dist1 = (cam1->getPosition() - small.center()).length();

    auto cam2 = CameraFactory::createPerspective(params);
    cam2->setPosition(Vec3f(0.0f, 0.0f, 20.0f));
    cam2->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam2->resize(800.0f, 600.0f);
    cam2->updateMatrices();
    Aabb large = Aabb::fromCenterAndHalfExtents(Vec3f(0, 0, 0), Vec3f(4.0f, 4.0f, 4.0f));
    fitToAabb(*cam2, large, 1.1f);
    float dist2 = (cam2->getPosition() - large.center()).length();

    EXPECT_GT(dist2, dist1);
}

TEST(CameraUtils, FitToAABB_ExtendsFarPlaneWhenNeeded) {
    // Use a tight far plane (5.0f) with a large AABB; fitToAabb must extend far to cover it.
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 5.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 0.0f, 20.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    Aabb aabb = Aabb::fromCenterAndHalfExtents(Vec3f(0, 0, 0), Vec3f(10.0f, 10.0f, 10.0f));
    float far_before = cam->getFarPlane();
    fitToAabb(*cam, aabb, 1.1f);

    // The required view distance should exceed far_before; far plane must have grown.
    EXPECT_GT(cam->getFarPlane(), far_before) << "fitToAabb must extend far plane when computed distance exceeds it";
    // AABB back face must still be within the frustum.
    Frustum f = buildFrustum(*cam);
    for (uint32_t i = 0; i < 8u; ++i) {
        EXPECT_TRUE(f.intersects(Aabb::fromCenterAndHalfExtents(aabb.corner(i), Vec3f(0.01f, 0.01f, 0.01f))))
            << "Corner " << i << " should be inside frustum after fitToAabb extends far plane";
    }
}
