#pragma once
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

/**
 * @file camera_transform_adapter.h
 * @brief Transform attachment policy: sync camera pose with vnemath TransformNode.
 *
 * Policy (Option 2): Camera keeps position/target/up; TransformNode is the
 * source of truth for hierarchy. Use these adapters to sync:
 * - syncCameraFromTransformNode: copy node world transform -> camera pose.
 * - syncTransformNodeFromCamera: copy camera pose -> node local transform.
 *
 * Long-term (Option 1): camera could reference a TransformNode and hold only
 * projection params; view direction would come from transform orientation.
 */

#include "vertexnova/scene/camera/camera.h"
#include <vertexnova/math/transform_node.h>
#include <vertexnova/math/transform_utils.h>
#include <vertexnova/math/core/core.h>

namespace vne::scene {

/**
 * @brief Copy TransformNode world transform into camera position/target/up.
 * @param camera The camera to update.
 * @param node The transform node (world matrix used).
 */
inline void syncCameraFromTransformNode(ICamera& camera, const vne::math::TransformNode& node) noexcept {
    vne::math::Mat4f world = node.getModelMatrix();
    vne::math::Vec3f position = vne::math::extractTranslation(world);
    vne::math::Vec3f col2(world.getColumn(2).x(), world.getColumn(2).y(), world.getColumn(2).z());
    vne::math::Vec3f forward = col2.normalized();
    vne::math::Vec3f col1(world.getColumn(1).x(), world.getColumn(1).y(), world.getColumn(1).z());
    vne::math::Vec3f up = col1.normalized();

    camera.setPosition(position);
    camera.setTarget(position + forward);
    camera.setUp(up);
}

/**
 * @brief Copy camera position/target/up into TransformNode local transform.
 * @param node The transform node to update (local transform set to camera world matrix).
 * @param camera The camera to read pose from.
 */
inline void syncTransformNodeFromCamera(vne::math::TransformNode& node, const ICamera& camera) noexcept {
    vne::math::Mat4f view = camera.getViewMatrix();
    vne::math::Mat4f camera_world = view.inverse();
    node.setLocalTransform(camera_world);
}

}  // namespace vne::scene
