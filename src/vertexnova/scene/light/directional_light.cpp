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

#include "vertexnova/scene/light/directional_light.h"
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

DirectionalLight::DirectionalLight(const Vec3f& direction, const Vec3f& color, float intensity, const std::string& name)
    : direction_(normalizeSafe(direction))
    , color_(color)
    , intensity_(intensity)
    , name_(name)
    , enabled_(true) {}

LightType DirectionalLight::getLightType() const noexcept {
    return LightType::eDirectional;
}
Vec3f DirectionalLight::getColor() const noexcept {
    return color_;
}
void DirectionalLight::setColor(const Vec3f& color) noexcept {
    color_ = color;
}
float DirectionalLight::getIntensity() const noexcept {
    return intensity_;
}
void DirectionalLight::setIntensity(float intensity) noexcept {
    intensity_ = std::max(0.0f, intensity);
}
Vec3f DirectionalLight::getPosition() const noexcept {
    return Vec3f(0.0f, 0.0f, 0.0f);
}
void DirectionalLight::setPosition(const Vec3f&) noexcept {}
Vec3f DirectionalLight::getDirection() const noexcept {
    return direction_;
}
void DirectionalLight::setDirection(const Vec3f& direction) noexcept {
    direction_ = normalizeSafe(direction);
}
bool DirectionalLight::isEnabled() const noexcept {
    return enabled_;
}
void DirectionalLight::setEnabled(bool enabled) noexcept {
    enabled_ = enabled;
}
const std::string& DirectionalLight::getName() const noexcept {
    return name_;
}
void DirectionalLight::setName(const std::string& name) noexcept {
    name_ = name;
}
void DirectionalLight::update(double) noexcept {}

LightGpu DirectionalLight::toGpu() const noexcept {
    LightGpu out{};
    out.color_intensity = Float4{color_.x(), color_.y(), color_.z(), std::max(0.0f, intensity_)};
    out.position_range = Float4{0.0f, 0.0f, 0.0f, 0.0f};
    out.direction_inner_cos = Float4{direction_.x(), direction_.y(), direction_.z(), 0.0f};
    out.misc = Float4{1.0f, enabled_ ? 1.0f : 0.0f, 0.0f, 0.0f};
    return out;
}

void DirectionalLight::setShadowCasting(bool shadow_casting) noexcept {
    shadow_.enabled = shadow_casting;
}
void DirectionalLight::setShadowMapSize(uint32_t size) noexcept {
    // Round up to next power of 2; cap at 2^31 to avoid overflow in the shift loop.
    uint32_t p2 = 1;
    while (p2 < size && p2 < (1u << 31))
        p2 <<= 1;
    shadow_.resolution = p2;
}
void DirectionalLight::setShadowBias(float bias) noexcept {
    shadow_.bias = std::max(0.0f, bias);
}
void DirectionalLight::setShadowFarPlane(float far_plane) noexcept {
    shadow_.far_plane = std::max(0.1f, far_plane);
}

}  // namespace vne::scene
