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

#include "vertexnova/scene/scene_state.h"
#include <algorithm>

namespace vne::scene {

void SceneState::setActiveCamera(CameraPtr camera) noexcept {
    active_camera_ = std::move(camera);
}

SceneState::CameraPtr SceneState::getActiveCamera() const noexcept {
    return active_camera_;
}

bool SceneState::hasActiveCamera() const noexcept {
    return active_camera_ != nullptr;
}

void SceneState::addLight(LightPtr light) {
    if (!light) {
        return;
    }
    lights_.push_back(std::move(light));
    // If max_lights_ is set, we keep only the last N (FIFO drop)
    if (max_lights_ > 0 && lights_.size() > max_lights_) {
        lights_.erase(lights_.begin(), lights_.begin() + static_cast<ptrdiff_t>(lights_.size() - max_lights_));
    }
}

bool SceneState::removeLight(const LightPtr& light) {
    auto it = std::find(lights_.begin(), lights_.end(), light);
    if (it == lights_.end()) {
        return false;
    }
    lights_.erase(it);
    return true;
}

void SceneState::clearLights() noexcept {
    lights_.clear();
}

const std::vector<SceneState::LightPtr>& SceneState::getLights() const noexcept {
    return lights_;
}

size_t SceneState::getLightCount() const noexcept {
    return lights_.size();
}

void SceneState::setMaxLights(size_t max_lights) noexcept {
    max_lights_ = max_lights;
    if (max_lights_ > 0 && lights_.size() > max_lights_) {
        lights_.erase(lights_.begin(), lights_.begin() + static_cast<ptrdiff_t>(lights_.size() - max_lights_));
    }
}

}  // namespace vne::scene
