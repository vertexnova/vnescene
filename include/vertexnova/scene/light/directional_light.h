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
 * @file directional_light.h
 * @brief Directional (e.g. sun) light implementation.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/light/light.h"
#include <vertexnova/math/core/core.h>
#include <cstdint>
#include <string>

namespace vne::scene {

/**
 * @class DirectionalLight
 * @brief Directional light with optional shadow (shadow map size, bias, far plane).
 *
 * setShadowMapSize() and setShadowSettings() both round resolution up to the next
 * power of 2 for consistency (e.g. 1000 → 1024).
 */
class VNE_SCENE_API DirectionalLight : public ILight {
   public:
    /**
     * @brief Construct directional light.
     * @param direction Light direction (typically toward scene).
     * @param color RGB color.
     * @param intensity Intensity factor.
     * @param name Light name.
     */
    DirectionalLight(const vne::math::Vec3f& direction,
                     const vne::math::Vec3f& color,
                     float intensity,
                     const std::string& name = "DirectionalLight");
    ~DirectionalLight() noexcept override = default;

    /** @brief Returns LightType::eDirectional. */
    [[nodiscard]] LightType getLightType() const noexcept override;
    /** @brief Get RGB color. */
    [[nodiscard]] vne::math::Vec3f getColor() const noexcept override;
    /** @brief Set RGB color. */
    void setColor(const vne::math::Vec3f& color) noexcept override;
    /** @brief Get intensity. */
    [[nodiscard]] float getIntensity() const noexcept override;
    /** @brief Set intensity. */
    void setIntensity(float intensity) noexcept override;
    /** @brief Directional has no position; returns (0,0,0). */
    [[nodiscard]] vne::math::Vec3f getPosition() const noexcept override;
    /** @brief No-op for directional. */
    void setPosition(const vne::math::Vec3f& position) noexcept override;
    /** @brief Get light direction. */
    [[nodiscard]] vne::math::Vec3f getDirection() const noexcept override;
    /** @brief Set light direction. */
    void setDirection(const vne::math::Vec3f& direction) noexcept override;
    /** @brief Whether the light is enabled. */
    [[nodiscard]] bool isEnabled() const noexcept override;
    /** @brief Set enabled state. */
    void setEnabled(bool enabled) noexcept override;
    /** @brief Get light name. */
    [[nodiscard]] const std::string& getName() const noexcept override;
    /** @brief Set light name. */
    void setName(const std::string& name) noexcept override;
    /** @brief Update light state (e.g. animation). */
    void update(double delta_time) noexcept override;
    /** @brief Pack as LightGpu for GPU (type = Directional). */
    [[nodiscard]] LightGpu toGpu() const noexcept override;

    [[nodiscard]] const ShadowSettings& getShadowSettings() const noexcept override { return shadow_; }
    void setShadowSettings(const ShadowSettings& settings) noexcept override {
        shadow_ = settings;
        // Apply same power-of-2 rounding as setShadowMapSize() for resolution.
        setShadowMapSize(settings.resolution);
    }

    /** @brief Whether this light casts shadows. */
    [[nodiscard]] bool isShadowCasting() const noexcept { return shadow_.enabled; }
    /** @brief Set whether this light casts shadows. */
    void setShadowCasting(bool shadow_casting) noexcept;
    /** @brief Get shadow map resolution (e.g. 1024). */
    [[nodiscard]] uint32_t getShadowMapSize() const noexcept { return shadow_.resolution; }
    /** @brief Set shadow map size; rounds up to the next power of 2 (e.g. 1000 → 1024). */
    void setShadowMapSize(uint32_t size) noexcept;
    /** @brief Get shadow depth bias. */
    [[nodiscard]] float getShadowBias() const noexcept { return shadow_.bias; }
    /** @brief Set shadow bias. */
    void setShadowBias(float bias) noexcept;
    /** @brief Get shadow far plane distance. */
    [[nodiscard]] float getShadowFarPlane() const noexcept { return shadow_.far_plane; }
    /** @brief Set shadow far plane. */
    void setShadowFarPlane(float far_plane) noexcept;

   private:
    vne::math::Vec3f direction_;  //!< Light direction (typically toward scene).
    vne::math::Vec3f color_;      //!< RGB color.
    float intensity_;             //!< Intensity factor.
    std::string name_;            //!< Light name.
    bool enabled_;                //!< Whether the light is enabled.
    ShadowSettings shadow_{};     //!< Directional shadow settings (data-only).
};

}  // namespace vne::scene
