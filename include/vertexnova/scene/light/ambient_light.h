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
 * @file ambient_light.h
 * @brief Ambient (non-directional) light implementation.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/light/light.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class AmbientLight
 * @brief Ambient light: uniform color and intensity (no position or direction).
 */
class VNE_SCENE_API AmbientLight : public ILight {
   public:
    /**
     * @brief Construct ambient light.
     * @param color RGB color.
     * @param intensity Intensity factor.
     * @param name Light name.
     */
    AmbientLight(const vne::math::Vec3f& color, float intensity, const std::string& name = "AmbientLight");
    ~AmbientLight() noexcept override = default;

    /** @brief Returns LightType::eAmbient. */
    [[nodiscard]] LightType getLightType() const noexcept override { return LightType::eAmbient; }
    /** @brief Get RGB color. */
    [[nodiscard]] vne::math::Vec3f getColor() const noexcept override { return color_; }
    /** @brief Set RGB color. */
    void setColor(const vne::math::Vec3f& color) noexcept override { color_ = color; }
    /** @brief Get intensity. */
    [[nodiscard]] float getIntensity() const noexcept override { return intensity_; }
    /** @brief Set intensity. */
    void setIntensity(float intensity) noexcept override;
    /** @brief Ambient has no position; returns (0,0,0). */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override { return vne::math::Vec3f(0.0f, 0.0f, 0.0f); }
    /** @brief No-op for ambient. */
    void setPosition(const vne::math::Vec3f&) noexcept override {}
    /** @brief Ambient has no direction; returns (0,-1,0). */
    [[nodiscard]] vne::math::Vec3f getDirection() const noexcept override {
        return vne::math::Vec3f(0.0f, -1.0f, 0.0f);
    }
    /** @brief No-op for ambient. */
    void setDirection(const vne::math::Vec3f&) noexcept override {}
    /** @brief Whether the light is enabled. */
    [[nodiscard]] bool isEnabled() const noexcept override { return enabled_; }
    /** @brief Set enabled state. */
    void setEnabled(bool enabled) noexcept override { enabled_ = enabled; }
    /** @brief Get light name. */
    [[nodiscard]] const std::string& getName() const noexcept override { return name_; }
    /** @brief Set light name. */
    void setName(const std::string& name) noexcept override { name_ = name; }
    /** @brief No-op for ambient. */
    void update(double) noexcept override {}
    /** @brief Pack as LightGpu for GPU (type = Ambient). */
    [[nodiscard]] LightGpu toGpu() const noexcept override;

    [[nodiscard]] const ShadowSettings& getShadowSettings() const noexcept override { return shadow_; }
    void setShadowSettings(const ShadowSettings& settings) noexcept override { shadow_ = settings; }

   private:
    vne::math::Vec3f color_;   //!< RGB color.
    float intensity_;          //!< Intensity factor.
    std::string name_;         //!< Light name.
    bool enabled_;             //!< Whether the light is enabled.
    ShadowSettings shadow_{};  //!< Ambient shadows are typically disabled; kept for uniform API.
};

}  // namespace vne::scene
