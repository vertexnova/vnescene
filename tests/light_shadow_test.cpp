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

#include <gtest/gtest.h>
#include "vertexnova/scene/light/light.h"
#include "vertexnova/scene/light/ambient_light.h"
#include "vertexnova/scene/light/directional_light.h"
#include "vertexnova/scene/light/spot_light.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

TEST(DirectionalLight_ShadowDefaultsDisabled, DefaultConstructed_HasShadowsDisabled) {
    DirectionalLight light(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, "sun");
    EXPECT_FALSE(light.getShadowSettings().enabled);
    EXPECT_FALSE(light.isShadowCasting());
}

TEST(DirectionalLight_SetShadowMapSize, RoundsUpToPow2) {
    DirectionalLight light(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, "sun");
    light.setShadowMapSize(1000u);
    EXPECT_EQ(light.getShadowMapSize(), 1024u);
    EXPECT_EQ(light.getShadowSettings().resolution, 1024u);

    light.setShadowMapSize(1024u);
    EXPECT_EQ(light.getShadowMapSize(), 1024u);
}

TEST(DirectionalLight_SetShadowSettings, RoundsResolutionToPow2) {
    DirectionalLight light(Vec3f(0.0f, -1.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f), 1.0f, "sun");
    ShadowSettings settings;
    settings.enabled = true;
    settings.resolution = 1000u;
    settings.bias = 0.01f;
    light.setShadowSettings(settings);

    EXPECT_EQ(light.getShadowMapSize(), 1024u);
    EXPECT_EQ(light.getShadowSettings().resolution, 1024u);
    EXPECT_TRUE(light.getShadowSettings().enabled);
    EXPECT_FLOAT_EQ(light.getShadowSettings().bias, 0.01f);
}

TEST(SpotLight_ShadowSettingsRoundTrip, SetAndGetShadowSettings) {
    SpotLight light(Vec3f(0.0f, 0.0f, 0.0f),
                    Vec3f(0.0f, -1.0f, 0.0f),
                    Vec3f(1.0f, 1.0f, 1.0f),
                    1.0f,
                    10.0f,
                    30.0f,
                    45.0f,
                    "spot");
    ShadowSettings settings;
    settings.enabled = true;
    settings.resolution = 2048u;
    settings.bias = 0.01f;
    light.setShadowSettings(settings);

    const ShadowSettings& got = light.getShadowSettings();
    EXPECT_TRUE(got.enabled);
    EXPECT_EQ(got.resolution, 2048u);
    EXPECT_FLOAT_EQ(got.bias, 0.01f);
}

TEST(AmbientLight_ShadowSettingsExistsButDisabledByDefault, DefaultThenSetEnabled) {
    AmbientLight light(Vec3f(1.0f, 1.0f, 1.0f), 0.2f, "ambient");
    EXPECT_FALSE(light.getShadowSettings().enabled);

    ShadowSettings settings;
    settings.enabled = true;
    light.setShadowSettings(settings);
    EXPECT_TRUE(light.getShadowSettings().enabled);
}
