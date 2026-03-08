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
#include "vertexnova/scene/environment.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

TEST(Environment, DefaultValues) {
    Environment env;
    EXPECT_FLOAT_EQ(env.exposure, 1.0f);
    EXPECT_FLOAT_EQ(env.gamma, 2.2f);
    EXPECT_FLOAT_EQ(env.clear_color.x(), 0.1f);
    EXPECT_FLOAT_EQ(env.clear_color.y(), 0.1f);
    EXPECT_FLOAT_EQ(env.clear_color.z(), 0.15f);
    EXPECT_FLOAT_EQ(env.clear_color_opacity, 1.0f);
    EXPECT_TRUE(env.hdri_asset_id.empty());
}

TEST(Environment, SetGetRoundTrip) {
    Environment env;
    env.exposure = 2.0f;
    env.gamma = 2.4f;
    EXPECT_FLOAT_EQ(env.exposure, 2.0f);
    EXPECT_FLOAT_EQ(env.gamma, 2.4f);

    env.setClearColor(Vec3f(0.2f, 0.4f, 0.6f));
    EXPECT_FLOAT_EQ(env.clear_color.x(), 0.2f);
    EXPECT_FLOAT_EQ(env.clear_color.y(), 0.4f);
    EXPECT_FLOAT_EQ(env.clear_color.z(), 0.6f);
    EXPECT_FLOAT_EQ(env.clear_color_opacity, 1.0f);

    env.setClearColor(Vec3f(0.1f, 0.2f, 0.3f), 0.5f);
    EXPECT_FLOAT_EQ(env.clear_color.x(), 0.1f);
    EXPECT_FLOAT_EQ(env.clear_color.y(), 0.2f);
    EXPECT_FLOAT_EQ(env.clear_color.z(), 0.3f);
    EXPECT_FLOAT_EQ(env.clear_color_opacity, 0.5f);

    env.setClearColor(Vec4f(0.5f, 0.5f, 0.5f, 0.8f));
    EXPECT_FLOAT_EQ(env.clear_color.x(), 0.5f);
    EXPECT_FLOAT_EQ(env.clear_color.y(), 0.5f);
    EXPECT_FLOAT_EQ(env.clear_color.z(), 0.5f);
    EXPECT_FLOAT_EQ(env.clear_color_opacity, 0.8f);

    Vec4f rgba = env.clearColorRgba();
    EXPECT_FLOAT_EQ(rgba.x(), 0.5f);
    EXPECT_FLOAT_EQ(rgba.y(), 0.5f);
    EXPECT_FLOAT_EQ(rgba.z(), 0.5f);
    EXPECT_FLOAT_EQ(rgba.w(), 0.8f);
}

TEST(Environment, CopyAssign) {
    Environment a;
    a.exposure = 1.5f;
    a.gamma = 2.0f;
    a.setClearColor(Vec3f(0.1f, 0.2f, 0.3f));
    a.hdri_asset_id = "test.hdr";

    Environment b(a);
    EXPECT_FLOAT_EQ(b.exposure, 1.5f);
    EXPECT_FLOAT_EQ(b.gamma, 2.0f);
    EXPECT_FLOAT_EQ(b.clear_color.x(), 0.1f);
    EXPECT_EQ(b.hdri_asset_id, "test.hdr");

    b.exposure = 3.0f;
    b.clear_color = Vec3f(1, 1, 1);
    EXPECT_FLOAT_EQ(a.exposure, 1.5f);
    EXPECT_FLOAT_EQ(a.clear_color.x(), 0.1f);

    Environment c;
    c = a;
    EXPECT_FLOAT_EQ(c.exposure, 1.5f);
    c.gamma = 1.0f;
    EXPECT_FLOAT_EQ(a.gamma, 2.0f);
}
