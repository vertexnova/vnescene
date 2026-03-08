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
 * @file light.h
 * @brief Light interface and type enumeration for the scene system.
 */

#include "vertexnova/scene/export.h"
#include "vertexnova/scene/light/light_gpu.h"
#include <vertexnova/math/core/core.h>
#include <cstdint>
#include <memory>
#include <string>

namespace vne::scene {

// ---------------- Shadow (data-only) ----------------

/**
 * @enum ShadowFilter
 * @brief Shadow filtering method (renderer decides how to implement).
 */
enum class ShadowFilter : uint8_t {
    eHard = 0,
    ePCF = 1,
    // Future options (renderer-side):
    // eVSM  = 2,
    // eEVSM = 3
};

/**
 * @struct ShadowCascadeSettings
 * @brief Directional-light cascade configuration (data-only).
 */
struct ShadowCascadeSettings {
    uint8_t num_cascades{0};    //!< 0 means "no cascades" (or shadows disabled).
    float split_lambda{0.5f};   //!< 0..1, practical default.
    float max_distance{50.0f};  //!< Max shadow distance in world units.
    bool stabilize{true};       //!< Stabilize cascades to reduce shimmering.
};

/**
 * @struct ShadowSettings
 * @brief Data-only shadow parameters for a light. No GPU resources here.
 */
struct ShadowSettings {
    bool enabled{false};        //!< Whether this light casts shadows.
    uint32_t resolution{1024};  //!< Shadow map resolution (renderer may clamp to pow2).
    ShadowFilter filter{ShadowFilter::ePCF};
    float bias{0.005f};                //!< Depth bias.
    float normal_bias{0.01f};          //!< Normal/slope bias.
    float far_plane{100.0f};           //!< Useful for directional/spot shadow frustum.
    ShadowCascadeSettings cascades{};  //!< Directional-only usage (safe to ignore otherwise).
};

/**
 * @enum LightType
 * @brief Specifies the type of light source.
 */
enum class LightType {
    eAmbient = 0,      //!< Ambient (non-directional) light.
    eDirectional = 1,  //!< Directional (e.g. sun) light.
    ePoint = 2,        //!< Point (omni) light with position and range.
    eSpot = 3,         //!< Spot light with position, direction, and cone angles.
};

/**
 * @class ILight
 * @brief Base light interface for all light types.
 *
 * Common interface: color, intensity, position/direction (as applicable),
 * enabled state, name, and update. toGpu() packs data for GPU (e.g. std140).
 */
class VNE_SCENE_API ILight {
   public:
    virtual ~ILight() noexcept = default;

    /** @brief Get light type (ambient, directional, point, spot). */
    [[nodiscard]] virtual LightType getLightType() const noexcept = 0;
    /** @brief Get RGB color. */
    [[nodiscard]] virtual vne::math::Vec3f getColor() const noexcept = 0;
    /** @brief Set RGB color. */
    virtual void setColor(const vne::math::Vec3f& color) noexcept = 0;
    /** @brief Get intensity. */
    [[nodiscard]] virtual float getIntensity() const noexcept = 0;
    /** @brief Set intensity. */
    virtual void setIntensity(float intensity) noexcept = 0;
    /** @brief Get position (meaningful for point/spot). */
    [[nodiscard]] virtual vne::math::Vec3f getPosition() const noexcept = 0;
    /** @brief Set position. */
    virtual void setPosition(const vne::math::Vec3f& position) noexcept = 0;
    /** @brief Get direction (meaningful for directional/spot). */
    [[nodiscard]] virtual vne::math::Vec3f getDirection() const noexcept = 0;
    /** @brief Set direction. */
    virtual void setDirection(const vne::math::Vec3f& direction) noexcept = 0;
    /** @brief Whether the light is enabled. */
    [[nodiscard]] virtual bool isEnabled() const noexcept = 0;
    /** @brief Set enabled state. */
    virtual void setEnabled(bool enabled) noexcept = 0;
    /** @brief Get light name. */
    [[nodiscard]] virtual const std::string& getName() const noexcept = 0;
    /** @brief Set light name. */
    virtual void setName(const std::string& name) noexcept = 0;
    /** @brief Update light state (e.g. animation); delta_time in seconds. */
    virtual void update(double delta_time) noexcept = 0;
    /** @brief Pack light data for GPU (std140 / Metal buffer layout). */
    [[nodiscard]] virtual LightGpu toGpu() const noexcept = 0;

    /** @brief Get shadow settings (data-only; renderer consumes later). */
    [[nodiscard]] virtual const ShadowSettings& getShadowSettings() const noexcept = 0;
    /** @brief Set shadow settings. */
    virtual void setShadowSettings(const ShadowSettings& settings) noexcept = 0;
};

}  // namespace vne::scene
