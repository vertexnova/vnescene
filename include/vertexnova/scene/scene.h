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
 * @file scene.h
 * @brief Main include for the vnescene module: cameras and lights.
 *
 * This header pulls in the full scene API:
 * - Camera interfaces and types (ICamera, CameraType, CameraParameters)
 * - Camera implementations (PerspectiveCamera, OrthographicCamera)
 * - Camera factory (CameraFactory)
 * - Light interface and GPU layout (ILight, LightGpu, LightType)
 * - Light implementations (AmbientLight, DirectionalLight, PointLight, SpotLight)
 * - SceneState (active camera + lights), Environment, camera_utils, transform adapter, CameraGpu
 *
 * Include this file when you need the complete scene graph building blocks
 * for view (cameras) and lighting.
 */

#include "vertexnova/scene/handle.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/camera/camera_types.h"
#include "vertexnova/scene/camera/perspective_camera.h"
#include "vertexnova/scene/camera/orthographic_camera.h"
#include "vertexnova/scene/camera/camera_factory.h"
#include "vertexnova/scene/camera/camera_utils.h"
#include "vertexnova/scene/camera/camera_transform_adapter.h"
#include "vertexnova/scene/camera/camera_gpu.h"
#include "vertexnova/scene/light/light.h"
#include "vertexnova/scene/light/light_gpu.h"
#include "vertexnova/scene/light/ambient_light.h"
#include "vertexnova/scene/light/directional_light.h"
#include "vertexnova/scene/light/point_light.h"
#include "vertexnova/scene/light/spot_light.h"
#include "vertexnova/scene/scene_state.h"
#include "vertexnova/scene/environment.h"

namespace vne::scene {

/** @brief Pack light data for GPU (delegates to light.toGpu()). */
[[nodiscard]] inline LightGpu packLightGpu(const ILight& light) noexcept {
    return light.toGpu();
}

}  // namespace vne::scene
