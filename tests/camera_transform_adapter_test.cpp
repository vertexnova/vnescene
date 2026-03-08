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
#include "vertexnova/scene/camera/camera_transform_adapter.h"
#include "vertexnova/scene/camera/camera_factory.h"
#include "vertexnova/scene/camera/camera_types.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/transform_node.h>
#include <vertexnova/math/transform_utils.h>
#include <cmath>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTolerance = 1e-5f;

}  // namespace

TEST(CameraTransformAdapter, SyncCameraFromNode_MatchesExpectedPose) {
    TransformNode node;
    Vec3f pos(1.0f, 2.0f, 3.0f);
    node.setLocalTransform(Mat4f::translate(pos));

    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0, 0, 0));
    cam->setTarget(Vec3f(0, 0, -1));
    cam->updateMatrices();

    syncCameraFromTransformNode(*cam, node);
    Vec3f camPos = cam->getPosition();
    EXPECT_NEAR(camPos.x(), 1.0f, kTolerance);
    EXPECT_NEAR(camPos.y(), 2.0f, kTolerance);
    EXPECT_NEAR(camPos.z(), 3.0f, kTolerance);
}

TEST(CameraTransformAdapter, SyncNodeFromCamera_RoundTripStable) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    Vec3f pos(5.0f, 0.0f, 10.0f);
    Vec3f target(0.0f, 0.0f, 0.0f);
    Vec3f up(0.0f, 1.0f, 0.0f);
    cam->setPosition(pos);
    cam->setTarget(target);
    cam->setUp(up);
    cam->updateMatrices();

    TransformNode node;
    syncTransformNodeFromCamera(node, *cam);

    auto cam2 = CameraFactory::createPerspective(params);
    syncCameraFromTransformNode(*cam2, node);

    EXPECT_TRUE(cam2->getPosition().areSame(pos, kTolerance));
    Vec3f expectedForward = (target - pos).normalized();
    Vec3f actualForward = (cam2->getTarget() - cam2->getPosition()).normalized();
    float dot = expectedForward.x() * actualForward.x() + expectedForward.y() * actualForward.y()
                + expectedForward.z() * actualForward.z();
    EXPECT_GE(std::abs(dot), 0.99f);
}

TEST(CameraTransformAdapter, UpVectorHandling_NoRollDrift) {
    PerspectiveCameraParameters params(60.0f, 1.0f, 0.1f, 100.0f);
    auto cam = CameraFactory::createPerspective(params);
    cam->setPosition(Vec3f(0.0f, 2.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->updateMatrices();

    TransformNode node;
    syncTransformNodeFromCamera(node, *cam);
    syncCameraFromTransformNode(*cam, node);

    Vec3f up = cam->getUp();
    float len = std::sqrt(up.x() * up.x() + up.y() * up.y() + up.z() * up.z());
    EXPECT_NEAR(len, 1.0f, kTolerance);
    EXPECT_GT(up.y(), 0.5f);
}

TEST(CameraTransformAdapter, OrthographicAndPerspective_BothSupported) {
    PerspectiveCameraParameters pparams(60.0f, 1.0f, 0.1f, 100.0f);
    auto pers = CameraFactory::createPerspective(pparams);
    pers->setPosition(Vec3f(1.0f, 0.0f, 5.0f));
    pers->setTarget(Vec3f(0, 0, 0));
    pers->setUp(Vec3f(0, 1, 0));
    pers->updateMatrices();

    TransformNode nodeP;
    syncTransformNodeFromCamera(nodeP, *pers);
    auto pers2 = CameraFactory::createPerspective(pparams);
    syncCameraFromTransformNode(*pers2, nodeP);
    EXPECT_TRUE(pers2->getPosition().areSame(pers->getPosition(), kTolerance));

    OrthographicCameraParameters oparams(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    auto ortho = CameraFactory::createOrthographic(oparams);
    ortho->setPosition(Vec3f(0.0f, 0.0f, 10.0f));
    ortho->setTarget(Vec3f(0, 0, 0));
    ortho->setUp(Vec3f(0, 1, 0));
    ortho->updateMatrices();

    TransformNode nodeO;
    syncTransformNodeFromCamera(nodeO, *ortho);
    auto ortho2 = CameraFactory::createOrthographic(oparams);
    syncCameraFromTransformNode(*ortho2, nodeO);
    EXPECT_TRUE(ortho2->getPosition().areSame(ortho->getPosition(), kTolerance));
}
