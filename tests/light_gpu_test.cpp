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
#include "vertexnova/scene/light/light_gpu.h"
#include "vertexnova/scene/light/ambient_light.h"
#include "vertexnova/scene/light/point_light.h"
#include "vertexnova/scene/light/directional_light.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

namespace {

constexpr float kTolerance = 1e-5f;

}  // namespace

TEST(LightGpu, Layout_StaticAsserts) {
    EXPECT_EQ(sizeof(LightGpu), 64u);
    EXPECT_EQ(alignof(LightGpu), 16u);
}

TEST(LightGpu, ToGpu_PointLight_MapsFieldsCorrectly) {
    Vec3f pos(1.0f, 2.0f, 3.0f);
    Vec3f color(0.5f, 0.6f, 0.7f);
    float intensity = 1.2f;
    float range = 10.0f;
    PointLight light(pos, color, intensity, range, "point");

    LightGpu gpu = light.toGpu();

    EXPECT_NEAR(gpu.color_intensity.x, 0.5f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.y, 0.6f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.z, 0.7f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.w, intensity, kTolerance);

    EXPECT_NEAR(gpu.position_range.x, 1.0f, kTolerance);
    EXPECT_NEAR(gpu.position_range.y, 2.0f, kTolerance);
    EXPECT_NEAR(gpu.position_range.z, 3.0f, kTolerance);
    EXPECT_NEAR(gpu.position_range.w, range, kTolerance);

    EXPECT_NEAR(gpu.misc.x, 2.0f, kTolerance);
    EXPECT_NEAR(gpu.misc.y, 1.0f, kTolerance);
}

TEST(LightGpu, ToGpu_AmbientLight_MapsFieldsCorrectly) {
    Vec3f color(0.2f, 0.3f, 0.4f);
    float intensity = 0.5f;
    AmbientLight light(color, intensity, "ambient");

    LightGpu gpu = light.toGpu();

    EXPECT_NEAR(gpu.color_intensity.x, 0.2f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.y, 0.3f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.z, 0.4f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.w, intensity, kTolerance);

    EXPECT_NEAR(gpu.position_range.x, 0.0f, kTolerance);
    EXPECT_NEAR(gpu.position_range.y, 0.0f, kTolerance);
    EXPECT_NEAR(gpu.position_range.z, 0.0f, kTolerance);

    EXPECT_NEAR(gpu.misc.x, 0.0f, kTolerance);
    EXPECT_NEAR(gpu.misc.y, 1.0f, kTolerance);
}

TEST(LightGpu, ToGpu_DirectionalLight_MapsFieldsCorrectly) {
    Vec3f direction(0.0f, -1.0f, 0.0f);
    Vec3f color(1.0f, 1.0f, 0.9f);
    float intensity = 1.0f;
    DirectionalLight light(direction, color, intensity, "sun");

    LightGpu gpu = light.toGpu();

    EXPECT_NEAR(gpu.color_intensity.x, 1.0f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.y, 1.0f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.z, 0.9f, kTolerance);
    EXPECT_NEAR(gpu.color_intensity.w, intensity, kTolerance);

    EXPECT_NEAR(gpu.direction_inner_cos.x, 0.0f, kTolerance);
    EXPECT_NEAR(gpu.direction_inner_cos.y, -1.0f, kTolerance);
    EXPECT_NEAR(gpu.direction_inner_cos.z, 0.0f, kTolerance);

    EXPECT_NEAR(gpu.misc.x, 1.0f, kTolerance);
}
