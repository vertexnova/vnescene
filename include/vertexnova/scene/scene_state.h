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
 * @file scene_state.h
 * @brief Scene composition: active camera + lights (non-ECS, ECS-friendly).
 *
 * Holds the "what is currently in view" state: one active camera and a list
 * of lights. No input or GPU ownership; suitable for sandbox and viewer.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/camera/camera.h"
#include "vertexnova/scene/light/light.h"
#include <memory>
#include <vector>

namespace vne::scene {

/**
 * @class SceneState
 * @brief Container for active camera and lights (stable interface for viewer/sandbox).
 *
 * Responsibilities:
 * - setActiveCamera / getActiveCamera
 * - addLight / removeLight / getLights
 * - Optional: clamp to max N lights (selection policy)
 * - Optional: environment settings (see environment.h)
 */
class VNE_SCENE_API SceneState {
   public:
    using CameraPtr = std::shared_ptr<ICamera>;
    using LightPtr = std::shared_ptr<ILight>;

    SceneState() = default;
    ~SceneState() noexcept = default;

    SceneState(const SceneState&) = delete;
    SceneState& operator=(const SceneState&) = delete;
    SceneState(SceneState&&) noexcept = default;
    SceneState& operator=(SceneState&&) noexcept = default;

    // --- Active camera ---
    /** @brief Set the active camera (may be null). */
    void setActiveCamera(CameraPtr camera) noexcept;
    /** @brief Get the active camera (may be null). */
    [[nodiscard]] CameraPtr getActiveCamera() const noexcept;
    /** @brief True if there is an active camera. */
    [[nodiscard]] bool hasActiveCamera() const noexcept;

    // --- Lights ---
    /** @brief Add a light. If max_lights_ is set, may drop oldest beyond limit. */
    void addLight(LightPtr light);
    /** @brief Remove first occurrence of this light (by pointer). */
    bool removeLight(const LightPtr& light);
    /** @brief Remove all lights. */
    void clearLights() noexcept;
    /** @brief All lights (up to max_lights_ if set). */
    [[nodiscard]] const std::vector<LightPtr>& getLights() const noexcept;
    /** @brief Number of lights currently stored. */
    [[nodiscard]] size_t getLightCount() const noexcept;

    // --- Light selection policy (optional) ---
    /** @brief Set max number of lights (0 = no limit). Excess lights are removed (FIFO); only up to max_lights are
     * kept. */
    void setMaxLights(size_t max_lights) noexcept;
    /** @brief Get max lights (0 = no limit). */
    [[nodiscard]] size_t getMaxLights() const noexcept { return max_lights_; }

   private:
    CameraPtr active_camera_;
    std::vector<LightPtr> lights_;
    size_t max_lights_ = 0;  // 0 = no clamp
};

}  // namespace vne::scene
