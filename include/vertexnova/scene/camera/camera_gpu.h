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
 * @file camera_gpu.h
 * @brief GPU-friendly packed camera data (view, proj, viewProj, position, near/far, viewport, basis dirs).
 *
 * Includes orthonormal camera axes @c forward_dir, @c right_dir, and @c up_dir (each as @c Float4
 * with w=0) as part of the std140 / Metal buffer contract; @c packCameraGpu() and @c camera.toGpu()
 * populate them together with matrices and clip parameters.
 *
 * Data-only; compatible with std140 / Metal buffer layout. Use packCameraGpu()
 * to fill from an ICamera. Use camera.toGpu() or packCameraGpu(camera).
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/gpu_types.h"
#include <vertexnova/math/core/core.h>

namespace vne::scene {

class ICamera;

/**
 * @struct CameraGpu
 * @brief Packed camera data for GPU (view, proj, viewProj, position, near/far, viewport, basis).
 *
 * Matrices stored column-major (4 Float4 per matrix). @c forward_dir, @c right_dir, @c up_dir are
 * world-space unit directions (w=0). Layout compatible with std140 and typical Metal buffer usage.
 */
struct VNE_SCENE_API alignas(16) CameraGpu {
    Float4 view_col0;       //!< View matrix column 0.
    Float4 view_col1;       //!< View matrix column 1.
    Float4 view_col2;       //!< View matrix column 2.
    Float4 view_col3;       //!< View matrix column 3.
    Float4 proj_col0;       //!< Projection matrix column 0.
    Float4 proj_col1;       //!< Projection matrix column 1.
    Float4 proj_col2;       //!< Projection matrix column 2.
    Float4 proj_col3;       //!< Projection matrix column 3.
    Float4 view_proj_col0;  //!< View*Projection matrix column 0.
    Float4 view_proj_col1;  //!< View*Projection matrix column 1.
    Float4 view_proj_col2;  //!< View*Projection matrix column 2.
    Float4 view_proj_col3;  //!< View*Projection matrix column 3.
    Float4 position_near;   //!< Camera position (xyz), near plane (w).
    Float4 far_viewport;    //!< Far plane (x), viewport width (y), viewport height (z), unused (w).
    Float4 forward_dir;     //!< Camera forward (xyz), 0 (w). std140-friendly.
    Float4 right_dir;       //!< Camera right (xyz), 0 (w).
    Float4 up_dir;          //!< Camera orthonormal up (xyz), 0 (w).
};

static_assert(alignof(CameraGpu) == 16, "CameraGpu must be 16-byte aligned for GPU buffer layout");
static_assert(sizeof(CameraGpu) % 16 == 0, "CameraGpu size must be multiple of 16 for GPU buffer layout");
static_assert(
    sizeof(CameraGpu) == 272,
    "CameraGpu ABI size mismatch (expected 17 std140 vec4 slots): update shader/GPU layout when changing fields");

/**
 * @brief Build CameraGpu from precomputed matrices and parameters (shared by all camera types).
 * @param view View matrix.
 * @param proj Projection matrix.
 * @param view_proj Projection * view matrix (VP / clip-from-world; same product as @c proj * @c view).
 * @param position Camera position (xyz).
 * @param near_plane Near clip plane.
 * @param far_plane Far clip plane.
 * @param viewport_width Viewport/frustum width.
 * @param viewport_height Viewport/frustum height.
 * @return Filled CameraGpu.
 */
[[nodiscard]] VNE_SCENE_API CameraGpu makeCameraGpuFromMatricesAndParams(const vne::math::Mat4f& view,
                                                                         const vne::math::Mat4f& proj,
                                                                         const vne::math::Mat4f& view_proj,
                                                                         const vne::math::Vec3f& position,
                                                                         float near_plane,
                                                                         float far_plane,
                                                                         float viewport_width,
                                                                         float viewport_height) noexcept;

[[nodiscard]] VNE_SCENE_API CameraGpu makeCameraGpuFromMatricesAndParams(const vne::math::Mat4f& view,
                                                                         const vne::math::Mat4f& proj,
                                                                         const vne::math::Mat4f& view_proj,
                                                                         const vne::math::Vec3f& position,
                                                                         float near_plane,
                                                                         float far_plane,
                                                                         float viewport_width,
                                                                         float viewport_height,
                                                                         const vne::math::Vec3f& forward_dir,
                                                                         const vne::math::Vec3f& right_dir,
                                                                         const vne::math::Vec3f& up_dir) noexcept;

/**
 * @brief Fill CameraGpu from camera. Near/far/viewport from concrete type when available.
 * @param camera The camera (view/projection/position always; near/far/viewport best-effort).
 * @return Filled CameraGpu (near/far/viewport 0 if not available from interface).
 */
[[nodiscard]] VNE_SCENE_API CameraGpu packCameraGpu(const ICamera& camera) noexcept;

}  // namespace vne::scene
