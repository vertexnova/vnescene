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

    auto cam = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 100.0f));
    cam->setPosition(Vec3f(0.0f, 2.0f, 5.0f));
    VNE_LOG_INFO << "Camera position: " << cam->getPosition().x() << ", " << cam->getPosition().y() << ", "
                 << cam->getPosition().z();

    AmbientLight ambient(Vec3f(0.4f, 0.4f, 0.5f), 0.3f);
    DirectionalLight sun(Vec3f(0.5f, -1.0f, 0.3f), Vec3f(1.0f, 0.98f, 0.95f), 1.0f, "Sun");
    VNE_LOG_INFO << "Lights: " << ambient.getName() << ", " << sun.getName();

    return 0;
}
