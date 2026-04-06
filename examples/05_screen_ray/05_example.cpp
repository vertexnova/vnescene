/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "05_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

namespace vne::scene::examples {

namespace {

constexpr float kTol = 1e-4f;

}  // namespace

int runScreenRayExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("screenToWorldRay (viewport center)");
    auto cam = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
    cam->lookAt(Vec3f(0.0f, 0.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();

    float x_center = 400.0f;
    float y_center = 300.0f;
    Ray ray = screenToWorldRay(*cam, x_center, y_center, 800.0f, 600.0f, GraphicsApi::eOpenGL);

    VNE_LOG_INFO << "Screen center (" << x_center << ", " << y_center << ") -> ray origin: " << ray.origin().x() << ", "
                 << ray.origin().y() << ", " << ray.origin().z();
    VNE_LOG_INFO << "Ray direction: " << ray.direction().x() << ", " << ray.direction().y() << ", "
                 << ray.direction().z();

    if (!checkLengthNear("ray direction unit length", ray.direction(), 1.0f, kTol)) {
        ++failures;
    }
    if (!checkVec3Near("ray origin matches camera position", ray.origin(), cam->getPosition(), kTol)) {
        ++failures;
    }

    return failures;
}

}  // namespace vne::scene::examples
