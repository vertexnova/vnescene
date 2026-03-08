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
 * @file environment.h
 * @brief Environment settings (data only): tone, clear color, optional HDRI.
 *
 * Gives the renderer consistent exposure/gamma/clear knobs. No GPU textures;
 * HDRI is an asset id/handle for the application to resolve.
 */

#include <vertexnova/math/core/core.h>
#include <string>

namespace vne::scene {

/**
 * @struct Environment
 * @brief Data-only environment settings for rendering.
 *
 * Header-only struct: no VNE_SCENE_API to avoid Windows DLL import errors.
 * All methods are inline; no symbols are exported from the shared library.
 */
struct Environment {
    float exposure = 1.0f;                            //!< Exposure (linear scale).
    float gamma = 2.2f;                               //!< Gamma for tone mapping / output.
    vne::math::Vec3f clear_color{0.1f, 0.1f, 0.15f};  //!< Background/clear color (RGB).
    float clear_color_opacity = 1.0f;                 //!< Clear color alpha/opacity [0, 1].

    /** @brief Optional HDRI asset id (e.g. path or handle); not a GPU texture. */
    std::string hdri_asset_id;

    Environment() = default;

    /** @brief Set clear color from RGB (opacity = 1). */
    void setClearColor(const vne::math::Vec3f& rgb) noexcept {
        clear_color = rgb;
        clear_color_opacity = 1.0f;
    }

    /** @brief Set clear color from RGB and opacity. */
    void setClearColor(const vne::math::Vec3f& rgb, float opacity) noexcept {
        clear_color = rgb;
        clear_color_opacity = opacity;
    }

    /** @brief Set clear color from RGBA. */
    void setClearColor(const vne::math::Vec4f& rgba) noexcept {
        clear_color = rgba.xyz();
        clear_color_opacity = rgba.w();
    }

    /** @brief Get clear color as RGBA (e.g. for GPU upload). */
    [[nodiscard]] vne::math::Vec4f clearColorRgba() const noexcept {
        return vne::math::Vec4f(clear_color.x(), clear_color.y(), clear_color.z(), clear_color_opacity);
    }
};

}  // namespace vne::scene
