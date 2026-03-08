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

#include "vertexnova/scene/light/ambient_light.h"
#include <algorithm>

namespace vne::scene {

AmbientLight::AmbientLight(const vne::math::Vec3f& color, float intensity, const std::string& name)
    : color_(color)
    , intensity_(std::max(0.0f, intensity))
    , name_(name)
    , enabled_(true) {}

void AmbientLight::setIntensity(float intensity) noexcept {
    intensity_ = std::max(0.0f, intensity);
}

LightGpu AmbientLight::toGpu() const noexcept {
    LightGpu out{};
    out.color_intensity = Float4{color_.x(), color_.y(), color_.z(), intensity_};
    out.position_range = Float4{0.0f, 0.0f, 0.0f, 0.0f};
    out.direction_inner_cos = Float4{0.0f, -1.0f, 0.0f, 0.0f};
    out.misc = Float4{0.0f, enabled_ ? 1.0f : 0.0f, 0.0f, 0.0f};
    return out;
}

}  // namespace vne::scene
