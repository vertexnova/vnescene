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
#include "vertexnova/scene/scene_state.h"
#include "vertexnova/scene/camera/camera_factory.h"
#include "vertexnova/scene/camera/camera_types.h"
#include "vertexnova/scene/light/ambient_light.h"
#include "vertexnova/scene/light/point_light.h"
#include "vertexnova/scene/light/directional_light.h"
#include <vertexnova/math/core/core.h>

using namespace vne::scene;
using namespace vne::math;

TEST(SceneState, ActiveCamera_SetGet) {
    SceneState state;
    EXPECT_FALSE(state.hasActiveCamera());
    EXPECT_EQ(state.getActiveCamera(), nullptr);

    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    ASSERT_TRUE(cam);

    state.setActiveCamera(cam);
    EXPECT_TRUE(state.hasActiveCamera());
    EXPECT_EQ(state.getActiveCamera(), cam);

    state.setActiveCamera(nullptr);
    EXPECT_FALSE(state.hasActiveCamera());
    EXPECT_EQ(state.getActiveCamera(), nullptr);
}

TEST(SceneState, AddRemoveLights) {
    SceneState state;
    EXPECT_EQ(state.getLightCount(), 0u);
    EXPECT_TRUE(state.getLights().empty());

    auto ambient = std::make_shared<AmbientLight>(Vec3f(1, 1, 1), 0.5f, "ambient");
    auto point = std::make_shared<PointLight>(Vec3f(0, 1, 0), Vec3f(1, 0, 0), 1.0f, 10.0f, "point");
    auto dir = std::make_shared<DirectionalLight>(Vec3f(0, -1, 0), Vec3f(1, 1, 1), 1.0f, "sun");

    state.addLight(ambient);
    state.addLight(point);
    state.addLight(dir);
    EXPECT_EQ(state.getLightCount(), 3u);
    EXPECT_EQ(state.getLights().size(), 3u);
    EXPECT_EQ(state.getLights()[0], ambient);
    EXPECT_EQ(state.getLights()[1], point);
    EXPECT_EQ(state.getLights()[2], dir);

    bool removed = state.removeLight(point);
    EXPECT_TRUE(removed);
    EXPECT_EQ(state.getLightCount(), 2u);
    EXPECT_EQ(state.getLights()[0], ambient);
    EXPECT_EQ(state.getLights()[1], dir);

    EXPECT_FALSE(state.removeLight(point));
    EXPECT_EQ(state.getLightCount(), 2u);
}

TEST(SceneState, ClearLights) {
    SceneState state;
    state.addLight(std::make_shared<AmbientLight>(Vec3f(1, 1, 1), 0.5f, "a"));
    state.addLight(std::make_shared<PointLight>(Vec3f(0, 0, 0), Vec3f(1, 1, 1), 1.0f, 5.0f, "p"));
    EXPECT_EQ(state.getLightCount(), 2u);

    state.clearLights();
    EXPECT_EQ(state.getLightCount(), 0u);
    EXPECT_TRUE(state.getLights().empty());
}

TEST(SceneState, MaxLights_ClampsOrSelectsPolicy) {
    SceneState state;
    state.setMaxLights(2);
    EXPECT_EQ(state.getMaxLights(), 2u);

    auto a = std::make_shared<AmbientLight>(Vec3f(1, 0, 0), 0.5f, "A");
    auto b = std::make_shared<AmbientLight>(Vec3f(0, 1, 0), 0.5f, "B");
    auto c = std::make_shared<AmbientLight>(Vec3f(0, 0, 1), 0.5f, "C");

    state.addLight(a);
    state.addLight(b);
    state.addLight(c);
    EXPECT_EQ(state.getLightCount(), 2u);
    EXPECT_EQ(state.getLights()[0], b);
    EXPECT_EQ(state.getLights()[1], c);

    state.setMaxLights(0);
    state.clearLights();
    state.addLight(a);
    state.addLight(b);
    state.addLight(c);
    EXPECT_EQ(state.getLightCount(), 3u);
}

TEST(SceneState, DeterministicLightOrder) {
    SceneState state;
    auto a = std::make_shared<AmbientLight>(Vec3f(1, 0, 0), 0.5f, "A");
    auto b = std::make_shared<AmbientLight>(Vec3f(0, 1, 0), 0.5f, "B");
    auto c = std::make_shared<AmbientLight>(Vec3f(0, 0, 1), 0.5f, "C");

    state.addLight(a);
    state.addLight(b);
    state.addLight(c);
    const auto& lights = state.getLights();
    ASSERT_EQ(lights.size(), 3u);
    EXPECT_EQ(lights[0], a);
    EXPECT_EQ(lights[1], b);
    EXPECT_EQ(lights[2], c);

    state.removeLight(b);
    ASSERT_EQ(state.getLightCount(), 2u);
    EXPECT_EQ(state.getLights()[0], a);
    EXPECT_EQ(state.getLights()[1], c);
}

TEST(SceneState, MoveConstruct_TransfersOwnership) {
    SceneState src;
    PerspectiveCameraParameters params(60.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    auto cam = CameraFactory::createPerspective(params);
    src.setActiveCamera(cam);
    auto light = std::make_shared<AmbientLight>(Vec3f(1, 1, 1), 0.5f, "a");
    src.addLight(light);

    SceneState dst(std::move(src));

    EXPECT_TRUE(dst.hasActiveCamera());
    EXPECT_EQ(dst.getActiveCamera(), cam);
    EXPECT_EQ(dst.getLightCount(), 1u);
    EXPECT_EQ(dst.getLights()[0], light);

    // Source is in a valid but unspecified state — must not crash.
    EXPECT_FALSE(src.hasActiveCamera());
}

TEST(SceneState, MoveAssign_TransfersOwnership) {
    SceneState src;
    auto light = std::make_shared<AmbientLight>(Vec3f(1, 1, 1), 0.5f, "x");
    src.addLight(light);
    src.setMaxLights(5);

    SceneState dst;
    dst = std::move(src);

    EXPECT_EQ(dst.getLightCount(), 1u);
    EXPECT_EQ(dst.getMaxLights(), 5u);
    EXPECT_FALSE(src.hasActiveCamera());
    EXPECT_EQ(src.getLightCount(), 0u);
}

TEST(SceneState, AddLight_NullIgnored) {
    SceneState state;
    state.addLight(nullptr);
    EXPECT_EQ(state.getLightCount(), 0u);
}
