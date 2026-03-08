#pragma once
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

/**
 * @file gpu_types.h
 * @brief Shared GPU layout types (Float4) for camera and light packing.
 *
 * Used by camera_gpu.h and light_gpu.h; keeps camera_gpu independent of light types.
 */

#include "vertexnova/scene/export.h"

namespace vne::scene {

/**
 * @struct Float4
 * @brief Four floats with 16-byte alignment for GPU layout (std140 / Metal).
 */
struct VNE_SCENE_API alignas(16) Float4 {
    float x{0.0f};  //!< First component.
    float y{0.0f};  //!< Second component.
    float z{0.0f};  //!< Third component.
    float w{0.0f};  //!< Fourth component.
};

static_assert(sizeof(Float4) == 16, "Float4 must be 16 bytes");
static_assert(alignof(Float4) == 16, "Float4 must be 16-byte aligned");

}  // namespace vne::scene
