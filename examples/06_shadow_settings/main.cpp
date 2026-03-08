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

    DirectionalLight dir(Vec3f(0.5f, -1.0f, 0.3f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, "Sun");
    ShadowSettings dirSettings;
    dirSettings.enabled = true;
    dirSettings.resolution = 2048u;
    dirSettings.bias = 0.01f;
    dirSettings.far_plane = 200.0f;
    dir.setShadowSettings(dirSettings);

    VNE_LOG_INFO << "DirectionalLight shadow: enabled=" << dir.getShadowSettings().enabled
                 << " resolution=" << dir.getShadowSettings().resolution << " bias=" << dir.getShadowSettings().bias;

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

    VNE_LOG_INFO << "SpotLight shadow: enabled=" << spot.getShadowSettings().enabled
                 << " resolution=" << spot.getShadowSettings().resolution;

    return 0;
}
