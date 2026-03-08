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

#include "vertexnova/scene/light/point_light.h"
#include <algorithm>

namespace vne::scene {

namespace {

constexpr float kMinRange = 0.001f;
// LightGpu misc.x: 0=Ambient, 1=Directional, 2=Point, 3=Spot
constexpr float kLightTypePoint = 2.0f;

}  // namespace

PointLight::PointLight(
    const vne::math::Vec3f& position, const vne::math::Vec3f& color, float intensity, float range, std::string name)
    : position_(position)
    , color_(color)
    , intensity_(std::max(0.0f, intensity))
    , range_(std::max(kMinRange, range))
    , name_(std::move(name))
    , enabled_(true) {}

void PointLight::setIntensity(float intensity) noexcept {
    intensity_ = std::max(0.0f, intensity);
}
void PointLight::setRange(float range) noexcept {
    range_ = std::max(kMinRange, range);
}

LightGpu PointLight::toGpu() const noexcept {
    LightGpu out{};
    out.color_intensity = Float4{color_.x(), color_.y(), color_.z(), intensity_};
    out.position_range = Float4{position_.x(), position_.y(), position_.z(), range_};
    out.direction_inner_cos = Float4{0.0f, -1.0f, 0.0f, 0.0f};
    out.misc = Float4{kLightTypePoint, enabled_ ? 1.0f : 0.0f, 0.0f, 0.0f};
    return out;
}

}  // namespace vne::scene
