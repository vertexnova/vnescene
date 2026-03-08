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
 * @file point_light.h
 * @brief Point (omni) light with position and range.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/light/light.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class PointLight
 * @brief Point light: position, color, intensity, and range (distance falloff).
 */
class VNE_SCENE_API PointLight : public ILight {
   public:
    /**
     * @brief Construct point light.
     * @param position World-space position.
     * @param color RGB color.
     * @param intensity Intensity factor.
     * @param range Maximum influence distance.
     * @param name Light name.
     */
    PointLight(const vne::math::Vec3f& position,
               const vne::math::Vec3f& color,
               float intensity,
               float range,
               const std::string& name = "PointLight");
    ~PointLight() noexcept override = default;

    /** @brief Returns LightType::ePoint. */
    [[nodiscard]] LightType getLightType() const noexcept override { return LightType::ePoint; }
    /** @brief Get RGB color. */
    [[nodiscard]] vne::math::Vec3f getColor() const noexcept override { return color_; }
    /** @brief Set RGB color. */
    void setColor(const vne::math::Vec3f& color) noexcept override { color_ = color; }
    /** @brief Get intensity. */
    [[nodiscard]] float getIntensity() const noexcept override { return intensity_; }
    /** @brief Set intensity. */
    void setIntensity(float intensity) noexcept override;
    /** @brief Get world-space position. */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override { return position_; }
    /** @brief Set position. */
    void setPosition(const vne::math::Vec3f& position) noexcept override { position_ = position; }
    /** @brief Point light has no direction; returns (0,-1,0). */
    [[nodiscard]] vne::math::Vec3f getDirection() const noexcept override {
        return vne::math::Vec3f(0.0f, -1.0f, 0.0f);
    }
    /** @brief No-op for point light. */
    void setDirection(const vne::math::Vec3f&) noexcept override {}
    /** @brief Whether the light is enabled. */
    [[nodiscard]] bool isEnabled() const noexcept override { return enabled_; }
    /** @brief Set enabled state. */
    void setEnabled(bool enabled) noexcept override { enabled_ = enabled; }
    /** @brief Get light name. */
    [[nodiscard]] const std::string& getName() const noexcept override { return name_; }
    /** @brief Set light name. */
    void setName(const std::string& name) noexcept override { name_ = name; }
    /** @brief No-op for point light. */
    void update(double) noexcept override {}
    /** @brief Pack as LightGpu for GPU (type = Point). */
    [[nodiscard]] LightGpu toGpu() const noexcept override;

    [[nodiscard]] const ShadowSettings& getShadowSettings() const noexcept override { return shadow_; }
    void setShadowSettings(const ShadowSettings& settings) noexcept override { shadow_ = settings; }

    /** @brief Get light range (max influence distance). */
    [[nodiscard]] float getRange() const noexcept { return range_; }
    /** @brief Set light range. */
    void setRange(float range) noexcept;

   private:
    vne::math::Vec3f position_;  //!< World-space position.
    vne::math::Vec3f color_;     //!< RGB color.
    float intensity_;            //!< Intensity factor.
    float range_;                //!< Maximum influence distance.
    std::string name_;           //!< Light name.
    bool enabled_;               //!< Whether the light is enabled.
    ShadowSettings shadow_{};    //!< Point shadows optional later (cube map, etc.).
};

}  // namespace vne::scene
