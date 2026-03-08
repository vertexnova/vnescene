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
 * @file spot_light.h
 * @brief Spot light with position, direction, cone angles, and range.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/light/light.h"
#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @class SpotLight
 * @brief Spot light: position, direction, inner/outer cone angles (degrees), and range.
 */
class VNE_SCENE_API SpotLight : public ILight {
   public:
    /**
     * @brief Construct spot light.
     * @param position World-space position.
     * @param direction Light direction (unit length).
     * @param color RGB color.
     * @param intensity Intensity factor.
     * @param range Maximum influence distance.
     * @param inner_angle_deg Inner cone angle (degrees); full intensity inside.
     * @param outer_angle_deg Outer cone angle (degrees); falloff to zero at edge.
     * @param name Light name.
     */
    SpotLight(const vne::math::Vec3f& position,
              const vne::math::Vec3f& direction,
              const vne::math::Vec3f& color,
              float intensity,
              float range,
              float inner_angle_deg,
              float outer_angle_deg,
              std::string name = "SpotLight");
    ~SpotLight() noexcept override = default;

    /** @brief Returns LightType::eSpot. */
    [[nodiscard]] LightType getLightType() const noexcept override { return LightType::eSpot; }
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
    /** @brief Get light direction. */
    [[nodiscard]] vne::math::Vec3f getDirection() const noexcept override { return direction_; }
    /** @brief Set light direction. */
    void setDirection(const vne::math::Vec3f& direction) noexcept override;
    /** @brief Whether the light is enabled. */
    [[nodiscard]] bool isEnabled() const noexcept override { return enabled_; }
    /** @brief Set enabled state. */
    void setEnabled(bool enabled) noexcept override { enabled_ = enabled; }
    /** @brief Get light name. */
    [[nodiscard]] const std::string& getName() const noexcept override { return name_; }
    /** @brief Set light name. */
    void setName(const std::string& name) noexcept override { name_ = name; }
    /** @brief No-op for spot light. */
    void update([[maybe_unused]] double delta_time) noexcept override {}
    /** @brief Pack as LightGpu for GPU (type = Spot). */
    [[nodiscard]] LightGpu toGpu() const noexcept override;

    [[nodiscard]] const ShadowSettings& getShadowSettings() const noexcept override { return shadow_; }
    void setShadowSettings(const ShadowSettings& settings) noexcept override { shadow_ = settings; }

    /** @brief Get light range (max influence distance). */
    [[nodiscard]] float getRange() const noexcept { return range_; }
    /** @brief Set light range. */
    void setRange(float range) noexcept;
    /** @brief Get inner cone angle in degrees (full intensity inside). */
    [[nodiscard]] float getInnerAngleDeg() const noexcept { return inner_angle_deg_; }
    /** @brief Get outer cone angle in degrees (falloff to zero at edge). */
    [[nodiscard]] float getOuterAngleDeg() const noexcept { return outer_angle_deg_; }
    /** @brief Set inner and outer cone angles (degrees). */
    void setInnerOuterAnglesDeg(float inner_angle_deg, float outer_angle_deg) noexcept;

   private:
    vne::math::Vec3f position_;   //!< World-space position.
    vne::math::Vec3f direction_;  //!< Light direction (unit length).
    vne::math::Vec3f color_;      //!< RGB color.
    float intensity_;             //!< Intensity factor.
    float range_;                 //!< Maximum influence distance.
    float inner_angle_deg_;       //!< Inner cone angle (degrees); full intensity inside.
    float outer_angle_deg_;       //!< Outer cone angle (degrees); falloff to zero at edge.
    std::string name_;            //!< Light name.
    bool enabled_;                //!< Whether the light is enabled.
    ShadowSettings shadow_{};     //!< Spot shadow settings (renderer consumes later).
};

}  // namespace vne::scene
