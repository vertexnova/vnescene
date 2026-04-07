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
 * The camera uses a quaternion-backed orientation API; @c setPosition / @c setTarget / @c setUp
 * drive the same pose state. TransformNode is the source of truth for hierarchy. Use these
 * adapters to sync:
 * - syncCameraFromTransformNode: copy node world transform -> camera position/target/up.
 * - syncTransformNodeFromCamera: copy camera world pose -> node local transform.
 *
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
    constexpr float kEps = 1e-6f;
    vne::math::Mat4f world = node.getModelMatrix();
    vne::math::Vec3f position = vne::math::extractTranslation(world);
    // Camera-to-world col2 is the camera's +Z axis (back direction in right-handed OpenGL).
    // Forward (toward scene) is -Z, so negate col2.
    const auto c2 = world.getColumn(2);
    vne::math::Vec3f back_axis(c2.x(), c2.y(), c2.z());
    const float len2 = back_axis.length();
    vne::math::Vec3f forward;
    if (len2 > kEps) {
        forward = -back_axis / len2;
    } else {
        forward = vne::math::Vec3f(0.0f, 0.0f, -1.0f);
    }

    const auto c1 = world.getColumn(1);
    vne::math::Vec3f up_axis(c1.x(), c1.y(), c1.z());
    const float len1 = up_axis.length();
    vne::math::Vec3f up;
    if (len1 > kEps) {
        up = up_axis / len1;
    } else {
        vne::math::Vec3f ref(0.0f, 1.0f, 0.0f);
        vne::math::Vec3f right = forward.cross(ref);
        float rl = right.length();
        if (rl < kEps) {
            ref = vne::math::Vec3f(1.0f, 0.0f, 0.0f);
            right = forward.cross(ref);
            rl = right.length();
        }
        if (rl > kEps) {
            right /= rl;
            up = right.cross(forward).normalized();
        } else {
            up = vne::math::Vec3f(0.0f, 1.0f, 0.0f);
        }
    }

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
