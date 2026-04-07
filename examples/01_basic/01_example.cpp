/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "01_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

namespace vne::scene::examples {

namespace {

constexpr float kTol = 1e-4f;

}  // namespace

int runBasicExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("Perspective camera (factory + quaternion pose)");
    auto persp = CameraFactory::createPerspective(PerspectiveCameraParameters(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f));
    persp->lookAt(Vec3f(0.0f, 2.0f, 5.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    persp->updateMatrices();

    VNE_LOG_INFO << "Camera position: " << persp->getPosition().x() << ", " << persp->getPosition().y() << ", "
                 << persp->getPosition().z();
    if (!checkLengthNear("perspective getForwardDir unit", persp->getForwardDir(), 1.0f, kTol)) {
        ++failures;
    }
    if (!checkNear("perspective quaternion normalized", persp->getOrientation().normalized().length(), 1.0f, kTol)) {
        ++failures;
    }
    const Vec3f fwd = persp->getForwardDir();
    const Vec3f tgt_minus_eye = (persp->getTarget() - persp->getPosition()).normalized();
    if (!checkVec3Near("forward matches derived target direction", fwd, tgt_minus_eye, kTol)) {
        ++failures;
    }
    VNE_LOG_INFO << "getUp() hint vs getUpDir() basis: hint=(" << persp->getUp().x() << "," << persp->getUp().y() << ","
                 << persp->getUp().z() << ") upDir=(" << persp->getUpDir().x() << "," << persp->getUpDir().y() << ","
                 << persp->getUpDir().z() << ")";

    printSection("Orthographic camera");
    auto ortho =
        CameraFactory::createOrthographic(OrthographicCameraParameters(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f));
    ortho->lookAt(Vec3f(0.0f, 0.0f, 10.0f), Vec3f(0.0f, 0.0f, 0.0f), Vec3f(0.0f, 1.0f, 0.0f));
    ortho->updateMatrices();
    if (!checkLengthNear("orthographic getForwardDir unit", ortho->getForwardDir(), 1.0f, kTol)) {
        ++failures;
    }

    printSection("Lights");
    AmbientLight ambient(Vec3f(0.4f, 0.4f, 0.5f), 0.3f);
    DirectionalLight sun(Vec3f(0.5f, -1.0f, 0.3f), Vec3f(1.0f, 0.98f, 0.95f), 1.0f, "Sun");
    VNE_LOG_INFO << "Lights: " << ambient.getName() << ", " << sun.getName();

    return failures;
}

}  // namespace vne::scene::examples
