/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "02_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

#include <memory>

namespace vne::scene::examples {

namespace {

constexpr float kTol = 1e-4f;

bool checkFloat4Vec(const char* label, float ax, float ay, float az, const vne::math::Vec3f& e, float tol) {
    const vne::math::Vec3f a(ax, ay, az);
    return checkVec3Near(label, a, e, tol);
}

}  // namespace

int runSceneGpuPackExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("SceneState + GPU pack");
    SceneState state;

    auto cam = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
    cam->setPosition(Vec3f(0.0f, 2.0f, 5.0f));
    cam->setTarget(Vec3f(0.0f, 0.0f, 0.0f));
    cam->resize(800.0f, 600.0f);
    cam->updateMatrices();
    state.setActiveCamera(cam);

    state.addLight(std::make_shared<AmbientLight>(Vec3f(0.3f, 0.3f, 0.4f), 0.2f));
    state.addLight(
        std::make_shared<DirectionalLight>(Vec3f(0.5f, -1.0f, 0.3f), Vec3f(1.0f, 0.98f, 0.95f), 1.0f, "Sun"));
    state.addLight(
        std::make_shared<PointLight>(Vec3f(2.0f, 1.0f, 2.0f), Vec3f(1.0f, 0.9f, 0.8f), 1.0f, 20.0f, "Point"));

    CameraGpu cam_gpu = state.getActiveCamera()->toGpu();
    VNE_LOG_INFO << "Camera GPU packed: position_near.w (near) = " << cam_gpu.position_near.w
                 << ", far_viewport.x (far) = " << cam_gpu.far_viewport.x;

    const ICamera& active = *state.getActiveCamera();
    if (!checkFloat4Vec("CameraGpu.forward_dir vs getForwardDir",
                        cam_gpu.forward_dir.x,
                        cam_gpu.forward_dir.y,
                        cam_gpu.forward_dir.z,
                        active.getForwardDir(),
                        kTol)) {
        ++failures;
    }
    if (!checkFloat4Vec("CameraGpu.right_dir vs getRightDir",
                        cam_gpu.right_dir.x,
                        cam_gpu.right_dir.y,
                        cam_gpu.right_dir.z,
                        active.getRightDir(),
                        kTol)) {
        ++failures;
    }
    if (!checkFloat4Vec("CameraGpu.up_dir vs getUpDir",
                        cam_gpu.up_dir.x,
                        cam_gpu.up_dir.y,
                        cam_gpu.up_dir.z,
                        active.getUpDir(),
                        kTol)) {
        ++failures;
    }

    printSection("Lights -> LightGpu");
    for (size_t i = 0; i < state.getLightCount(); ++i) {
        LightGpu light_gpu = packLightGpu(*state.getLights()[i]);
        VNE_LOG_INFO << "Light " << i << " GPU: color_intensity.w (intensity) = " << light_gpu.color_intensity.w;
    }

    return failures;
}

}  // namespace vne::scene::examples
