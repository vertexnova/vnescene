/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

#include "06_example.h"

#include "common/example_utils.h"
#include "vertexnova/scene/scene.h"

namespace vne::scene::examples {

int runShadowSettingsExample() {
    LoggingGuard logging_guard;
    using namespace vne::scene;
    using namespace vne::math;

    int failures = 0;

    printSection("DirectionalLight shadow settings");
    DirectionalLight dir(Vec3f(0.5f, -1.0f, 0.3f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, "Sun");
    ShadowSettings dirSettings;
    dirSettings.enabled = true;
    dirSettings.resolution = 2048u;
    dirSettings.bias = 0.01f;
    dirSettings.far_plane = 200.0f;
    dir.setShadowSettings(dirSettings);

    const ShadowSettings dirRead = dir.getShadowSettings();
    VNE_LOG_INFO << "DirectionalLight shadow: enabled=" << dirRead.enabled << " resolution=" << dirRead.resolution
                 << " bias=" << dirRead.bias;

    if (!checkTrue("directional shadow enabled", dirRead.enabled)) {
        ++failures;
    }
    if (!checkNear("directional shadow resolution", static_cast<float>(dirRead.resolution), 2048.0f, 0.5f)) {
        ++failures;
    }
    if (!checkNear("directional shadow bias", dirRead.bias, 0.01f, 1e-5f)) {
        ++failures;
    }

    printSection("SpotLight shadow settings");
    SpotLight spot(Vec3f(0.0f, 5.0f, 0.0f),
                   Vec3f(0.0f, -1.0f, 0.0f),
                   Vec3f(1.0f, 1.0f, 1.0f),
                   1.0f,
                   15.0f,
                   30.0f,
                   45.0f,
                   "Spot");
    ShadowSettings spotSettings;
    spotSettings.enabled = true;
    spotSettings.resolution = 1024u;
    spot.setShadowSettings(spotSettings);

    const ShadowSettings spotRead = spot.getShadowSettings();
    VNE_LOG_INFO << "SpotLight shadow: enabled=" << spotRead.enabled << " resolution=" << spotRead.resolution;

    if (!checkTrue("spot shadow enabled", spotRead.enabled)) {
        ++failures;
    }
    if (!checkNear("spot shadow resolution", static_cast<float>(spotRead.resolution), 1024.0f, 0.5f)) {
        ++failures;
    }

    return failures;
}

}  // namespace vne::scene::examples
