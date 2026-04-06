/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "03_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

namespace vne::scene::examples {

int runCameraBackendExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("Graphics API — projection Y convention");
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

    // Perspective Y scale should flip sign between OpenGL and Vulkan NDC conventions.
    const float prod = proj_opengl[1][1] * proj_vulkan[1][1];
    if (!checkTrue("OpenGL vs Vulkan proj[1][1] have opposite signs", prod < 0.0f)) {
        ++failures;
    }

    return failures;
}

}  // namespace vne::scene::examples
