/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "07_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

#include <vertexnova/math/transform_node.h>

namespace vne::scene::examples {

namespace {

constexpr float kTol = 1e-4f;

}  // namespace

int runCameraTransformSyncExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("Camera -> TransformNode");
    auto cam = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
    cam->lookAt(Vec3f(1.0f, 2.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    cam->updateMatrices();

    const Vec3f eye = cam->getPosition();

    TransformNode node;
    syncTransformNodeFromCamera(node, *cam);

    const Mat4f node_world = node.getModelMatrix();
    const Vec3f node_pos(node_world.getColumn(3).x(), node_world.getColumn(3).y(), node_world.getColumn(3).z());
    VNE_LOG_INFO << "Camera -> Node: node world position = " << node_pos.x() << ", " << node_pos.y() << ", "
                 << node_pos.z();

    if (!checkVec3Near("node translation matches camera position after syncTransformNodeFromCamera",
                       node_pos,
                       eye,
                       kTol)) {
        ++failures;
    }

    printSection("TransformNode -> Camera");
    node.setLocalTransform(Mat4f::translate(Vec3f(-1.0f, 3.0f, 4.0f)));
    syncCameraFromTransformNode(*cam, node);

    VNE_LOG_INFO << "Node -> Camera: camera position = " << cam->getPosition().x() << ", " << cam->getPosition().y()
                 << ", " << cam->getPosition().z();

    if (!checkVec3Near("camera position matches node translation after syncCameraFromTransformNode",
                       cam->getPosition(),
                       Vec3f(-1.0f, 3.0f, 4.0f),
                       kTol)) {
        ++failures;
    }

    return failures;
}

}  // namespace vne::scene::examples
