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
#include <memory>

int main() {
    vne::scene::examples::LoggingGuard logging_guard;

    using namespace vne::scene;
    using namespace vne::math;

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

    return 0;
}
