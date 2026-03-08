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
 * @file light_gpu.h
 * @brief GPU-friendly packed light data (std140 / Metal buffer compatible).
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/gpu_types.h"
#include <cstdint>

namespace vne::scene {

/**
 * @struct LightGpu
 * @brief Packed light data for GPU (64 bytes).
 *
 * type in misc: 0=Ambient, 1=Directional, 2=Point, 3=Spot.
 * Layout is compatible with std140 and typical Metal buffer usage.
 */
struct VNE_SCENE_API alignas(16) LightGpu {
    Float4 color_intensity;      //!< RGB + intensity.
    Float4 position_range;       //!< Position (xyz) + range (w).
    Float4 direction_inner_cos;  //!< Direction (xyz) + inner cone (w); spot only.
    Float4 misc;                 //!< Type, flags, outer cone, etc.
};

static_assert(sizeof(LightGpu) == 64, "LightGpu must be 64 bytes for GPU buffer layout");
static_assert(alignof(LightGpu) == 16, "LightGpu must be 16-byte aligned for GPU buffer layout");

}  // namespace vne::scene
