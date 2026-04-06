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

#include "vertexnova/scene/camera/camera_gpu.h"
#include "vertexnova/scene/camera/camera.h"
#include <vertexnova/math/core/core.h>

namespace vne::scene {

namespace {

void copyMat4ToFloat4Columns(const vne::math::Mat4f& m, Float4& c0, Float4& c1, Float4& c2, Float4& c3) noexcept {
    const auto& col0 = m.getColumn(0);
    const auto& col1 = m.getColumn(1);
    const auto& col2 = m.getColumn(2);
    const auto& col3 = m.getColumn(3);
    c0.x = col0.x();
    c0.y = col0.y();
    c0.z = col0.z();
    c0.w = col0.w();
    c1.x = col1.x();
    c1.y = col1.y();
    c1.z = col1.z();
    c1.w = col1.w();
    c2.x = col2.x();
    c2.y = col2.y();
    c2.z = col2.z();
    c2.w = col2.w();
    c3.x = col3.x();
    c3.y = col3.y();
    c3.z = col3.z();
    c3.w = col3.w();
}

}  // namespace

CameraGpu makeCameraGpuFromMatricesAndParams(const vne::math::Mat4f& view,
                                             const vne::math::Mat4f& proj,
                                             const vne::math::Mat4f& view_proj,
                                             const vne::math::Vec3f& position,
                                             float near_plane,
                                             float far_plane,
                                             float viewport_width,
                                             float viewport_height) noexcept {
    CameraGpu out{};
    copyMat4ToFloat4Columns(view, out.view_col0, out.view_col1, out.view_col2, out.view_col3);
    copyMat4ToFloat4Columns(proj, out.proj_col0, out.proj_col1, out.proj_col2, out.proj_col3);
    copyMat4ToFloat4Columns(view_proj, out.view_proj_col0, out.view_proj_col1, out.view_proj_col2, out.view_proj_col3);
    out.position_near.x = position.x();
    out.position_near.y = position.y();
    out.position_near.z = position.z();
    out.position_near.w = near_plane;
    out.far_viewport.x = far_plane;
    out.far_viewport.y = viewport_width;
    out.far_viewport.z = viewport_height;
    out.far_viewport.w = 0.0f;
    out.forward_dir.x = 0.0f;
    out.forward_dir.y = 0.0f;
    out.forward_dir.z = 0.0f;
    out.forward_dir.w = 0.0f;
    out.right_dir.x = 0.0f;
    out.right_dir.y = 0.0f;
    out.right_dir.z = 0.0f;
    out.right_dir.w = 0.0f;
    out.up_dir.x = 0.0f;
    out.up_dir.y = 0.0f;
    out.up_dir.z = 0.0f;
    out.up_dir.w = 0.0f;
    return out;
}

CameraGpu makeCameraGpuFromMatricesAndParams(const vne::math::Mat4f& view,
                                             const vne::math::Mat4f& proj,
                                             const vne::math::Mat4f& view_proj,
                                             const vne::math::Vec3f& position,
                                             float near_plane,
                                             float far_plane,
                                             float viewport_width,
                                             float viewport_height,
                                             const vne::math::Vec3f& forward_dir,
                                             const vne::math::Vec3f& right_dir,
                                             const vne::math::Vec3f& up_dir) noexcept {
    CameraGpu out = makeCameraGpuFromMatricesAndParams(
        view, proj, view_proj, position, near_plane, far_plane, viewport_width, viewport_height);
    out.forward_dir.x = forward_dir.x();
    out.forward_dir.y = forward_dir.y();
    out.forward_dir.z = forward_dir.z();
    out.forward_dir.w = 0.0f;
    out.right_dir.x = right_dir.x();
    out.right_dir.y = right_dir.y();
    out.right_dir.z = right_dir.z();
    out.right_dir.w = 0.0f;
    out.up_dir.x = up_dir.x();
    out.up_dir.y = up_dir.y();
    out.up_dir.z = up_dir.z();
    out.up_dir.w = 0.0f;
    return out;
}

CameraGpu packCameraGpu(const ICamera& camera) noexcept {
    return camera.toGpu();
}

}  // namespace vne::scene
