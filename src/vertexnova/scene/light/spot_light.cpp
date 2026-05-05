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

#include "vertexnova/scene/light/spot_light.h"
#include <vertexnova/logging/logging.h>
#include <vertexnova/math/core/constants.h>
#include <vertexnova/math/core/core.h>
#include <algorithm>
#include <cmath>

namespace vne::scene {

using namespace vne::math;

namespace {
CREATE_VNE_LOGGER_CATEGORY("vnescene.spot_light");
}  // namespace

static Vec3f normalizeSafe(const Vec3f& v) noexcept {
    float len = v.length();
    if (len <= kFloatEpsilon) {
        VNE_LOG_WARN << "SpotLight: near-zero direction vector, falling back to (0,-1,0)";
        return {0.0f, -1.0f, 0.0f};
    }
    return v / len;
}

namespace {

constexpr float kMaxSpotAngleDeg = 89.9f;
constexpr float kMinRange = 0.001f;
// LightGpu misc.x: 0=Ambient, 1=Directional, 2=Point, 3=Spot
constexpr float kLightTypeSpot = 3.0f;

}  // namespace

static float clampDeg(float deg) noexcept {
    return std::clamp(deg, 0.0f, kMaxSpotAngleDeg);
}

SpotLight::SpotLight(const Vec3f& position,
                     const Vec3f& direction,
                     const Vec3f& color,
                     float intensity,
                     float range,
                     float inner_angle_deg,
                     float outer_angle_deg,
                     std::string name)
    : position_(position)
    , direction_(normalizeSafe(direction))
    , color_(color)
    , intensity_(std::max(0.0f, intensity))
    , range_(std::max(kMinRange, range))
    , inner_angle_deg_(clampDeg(inner_angle_deg))
    , outer_angle_deg_(clampDeg(std::max(inner_angle_deg, outer_angle_deg)))
    , name_(std::move(name))
    , enabled_(true) {
    VNE_LOG_INFO << "SpotLight \"" << name_ << "\" created (range=" << range_ << ", inner=" << inner_angle_deg_
                 << "deg, outer=" << outer_angle_deg_ << "deg)";
}

void SpotLight::setIntensity(float intensity) noexcept {
    intensity_ = std::max(0.0f, intensity);
}
void SpotLight::setDirection(const Vec3f& direction) noexcept {
    direction_ = normalizeSafe(direction);
}
void SpotLight::setRange(float range) noexcept {
    range_ = std::max(kMinRange, range);
}
void SpotLight::setInnerOuterAnglesDeg(float inner_angle_deg, float outer_angle_deg) noexcept {
    inner_angle_deg_ = clampDeg(inner_angle_deg);
    outer_angle_deg_ = clampDeg(std::max(inner_angle_deg, outer_angle_deg));
}

LightGpu SpotLight::toGpu() const noexcept {
    float inner_cos = std::cos(degToRad(inner_angle_deg_));
    float outer_cos = std::cos(degToRad(outer_angle_deg_));
    LightGpu out{};
    out.color_intensity = Float4{color_.x(), color_.y(), color_.z(), intensity_};
    out.position_range = Float4{position_.x(), position_.y(), position_.z(), range_};
    out.direction_inner_cos = Float4{direction_.x(), direction_.y(), direction_.z(), inner_cos};
    out.misc = Float4{kLightTypeSpot, enabled_ ? 1.0f : 0.0f, outer_cos, 0.0f};
    return out;
}

}  // namespace vne::scene
