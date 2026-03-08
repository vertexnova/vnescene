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

#include "vertexnova/scene/camera/camera_utils.h"
#include "vertexnova/scene/camera/perspective_camera.h"
#include <vertexnova/math/core/core.h>
#include <vertexnova/math/core/types.h>
#include <algorithm>
#include <cmath>

namespace vne::scene {

using namespace vne::math;

void fitToAabb(PerspectiveCamera& cam, const Aabb& aabb, float padding) noexcept {
    Vec3f center = aabb.center();
    Vec3f half = aabb.halfExtents();
    float radius = half.length();
    if (radius <= 0.0f) {
        return;
    }

    float v_fov_rad = degToRad(cam.getFieldOfView());
    float tan_half = std::tan(v_fov_rad * 0.5f);
    if (tan_half <= 0.0f) {
        return;
    }

    float distance = (radius * padding) / tan_half;

    Vec3f from_cam = cam.getPosition() - center;
    float len = from_cam.length();
    Vec3f direction = (len > kEpsilon<float>) ? from_cam.normalized() : Vec3f(0.0f, 0.0f, 1.0f);

    Vec3f new_position = center + direction * distance;
    cam.setTarget(center);
    cam.setPosition(new_position);
}

}  // namespace vne::scene
