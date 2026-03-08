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
#include <vertexnova/math/core/constants.h>
#include <vertexnova/math/core/core.h>
#include <algorithm>
#include <cmath>

namespace vne::scene {

using namespace vne::math;

static Vec3f normalizeSafe(const Vec3f& v) noexcept {
    float len = v.length();
    if (len <= kFloatEpsilon) {
        return Vec3f(0.0f, -1.0f, 0.0f);
    }
    return v / len;
}
static float clampDeg(float deg) noexcept {
    return std::clamp(deg, 0.0f, 89.9f);
}

SpotLight::SpotLight(const Vec3f& position,
                     const Vec3f& direction,
                     const Vec3f& color,
                     float intensity,
                     float range,
                     float inner_angle_deg,
                     float outer_angle_deg,
                     const std::string& name)
    : position_(position)
    , direction_(normalizeSafe(direction))
    , color_(color)
    , intensity_(std::max(0.0f, intensity))
    , range_(std::max(0.001f, range))
    , inner_angle_deg_(clampDeg(inner_angle_deg))
    , outer_angle_deg_(clampDeg(std::max(inner_angle_deg, outer_angle_deg)))
    , name_(name)
    , enabled_(true) {}

void SpotLight::setIntensity(float intensity) noexcept {
    intensity_ = std::max(0.0f, intensity);
}
void SpotLight::setDirection(const Vec3f& direction) noexcept {
    direction_ = normalizeSafe(direction);
}
void SpotLight::setRange(float range) noexcept {
    range_ = std::max(0.001f, range);
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
    out.misc = Float4{3.0f, enabled_ ? 1.0f : 0.0f, outer_cos, 0.0f};
    return out;
}

}  // namespace vne::scene
