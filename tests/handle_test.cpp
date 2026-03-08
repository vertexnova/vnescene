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
#include "vertexnova/scene/handle.h"

using namespace vne::scene;

TEST(SceneHandle, Invalid_IsNotValid) {
    CameraId invalid = CameraId::Invalid();
    EXPECT_FALSE(invalid.IsValid());
    EXPECT_EQ(invalid.Raw(), UINT64_MAX);

    LightId lightInvalid = LightId::Invalid();
    EXPECT_FALSE(lightInvalid.IsValid());
    EXPECT_EQ(lightInvalid.Raw(), UINT64_MAX);
}

TEST(SceneHandle, Make_RoundTrip) {
    CameraId id = CameraId::Make(3, 7);
    EXPECT_TRUE(id.IsValid());
    EXPECT_EQ(id.Slot(), 3u);
    EXPECT_EQ(id.Gen(), 7u);
    EXPECT_EQ(id.Raw(), (static_cast<uint64_t>(7) << 32) | 3u);
}

TEST(SceneHandle, Make_ReservesMax) {
    CameraId id = CameraId::Make(0xFFFFFFFFu, 0xFFFFFFFFu);
    EXPECT_TRUE(id.IsValid());
    EXPECT_NE(id.Raw(), UINT64_MAX);
    EXPECT_EQ(id.Raw(), 0u);
}

TEST(SceneHandle, Generation_DistinguishesSameSlot) {
    CameraId a = CameraId::Make(1, 0);
    CameraId b = CameraId::Make(1, 1);
    EXPECT_NE(a, b);
    EXPECT_EQ(a.Slot(), b.Slot());
    EXPECT_NE(a.Gen(), b.Gen());

    LightId la = LightId::Make(2, 0);
    LightId lb = LightId::Make(2, 1);
    EXPECT_NE(la, lb);
}

TEST(SceneHandle, Equality) {
    CameraId a = CameraId::Make(5, 10);
    CameraId b = CameraId::Make(5, 10);
    EXPECT_EQ(a, b);
    EXPECT_EQ(a.Raw(), b.Raw());

    CameraId c = CameraId::Make(5, 11);
    EXPECT_NE(a, c);
    CameraId d = CameraId::Make(6, 10);
    EXPECT_NE(a, d);
}
