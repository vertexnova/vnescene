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
#include <vertexnova/math/geometry/aabb.h>
#include <memory>

int main() {
    vne::scene::examples::LoggingGuard logging_guard;

    using namespace vne::scene;
    using namespace vne::math;

    Aabb aabb(Vec3f(-1.0f, -1.0f, -1.0f), Vec3f(1.0f, 1.0f, 1.0f));

    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = std::make_shared<PerspectiveCamera>(params.fov,
                                                   params.aspect_ratio,
                                                   params.near_plane,
                                                   params.far_plane,
                                                   "FitCam");
    cam->resize(800.0f, 600.0f);

    fitToAabb(*cam, aabb, 1.1f);

    const float vw = 800.0f;
    const float vh = 600.0f;
    for (int i = 0; i < 8; ++i) {
        Vec3f corner((i & 1) ? aabb.max().x() : aabb.min().x(),
                     (i & 2) ? aabb.max().y() : aabb.min().y(),
                     (i & 4) ? aabb.max().z() : aabb.min().z());
        Vec3f screen = project(*cam, corner, vw, vh, GraphicsApi::eOpenGL);
        VNE_LOG_INFO << "AABB corner " << i << " screen: " << screen.x() << ", " << screen.y() << ", depth "
                     << screen.z();
    }

    return 0;
}
