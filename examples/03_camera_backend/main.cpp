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

    cam->setGraphicsApi(GraphicsApi::eOpenGL);
    cam->updateMatrices();
    Mat4f proj_opengl = cam->getProjectionMatrix();
    VNE_LOG_INFO << "OpenGL projection [1][1] (Y scale): " << proj_opengl[1][1];

    cam->setGraphicsApi(GraphicsApi::eVulkan);
    cam->updateMatrices();
    Mat4f proj_vulkan = cam->getProjectionMatrix();
    VNE_LOG_INFO << "Vulkan projection [1][1] (Y scale, flipped): " << proj_vulkan[1][1];

    VNE_LOG_INFO << "getGraphicsApi() = " << static_cast<int>(cam->getGraphicsApi());

    return 0;
}
