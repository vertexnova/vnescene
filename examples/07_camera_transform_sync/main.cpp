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

#include "common/logging_guard.h"
#include "vertexnova/scene/scene.h"
#include <vertexnova/math/transform_node.h>
#include <vertexnova/math/core/core.h>
#include <memory>

int main() {
    vne::scene::examples::LoggingGuard logging_guard;

    using namespace vne::scene;
    using namespace vne::math;

    auto cam = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
    cam->setPosition(Vec3f(1.0f, 2.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->setUp(Vec3f(0.0f, 1.0f, 0.0f));
    cam->updateMatrices();

    TransformNode node;
    syncTransformNodeFromCamera(node, *cam);

    Vec3f node_pos(node.getModelMatrix().getColumn(3).x(),
                   node.getModelMatrix().getColumn(3).y(),
                   node.getModelMatrix().getColumn(3).z());
    VNE_LOG_INFO << "Camera -> Node: node world position = " << node_pos.x() << ", " << node_pos.y() << ", "
                 << node_pos.z();

    node.setLocalTransform(Mat4f::translate(Vec3f(-1.0f, 3.0f, 4.0f)));
    syncCameraFromTransformNode(*cam, node);

    VNE_LOG_INFO << "Node -> Camera: camera position = " << cam->getPosition().x() << ", " << cam->getPosition().y()
                 << ", " << cam->getPosition().z();

    return 0;
}
