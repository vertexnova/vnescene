#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

/**
 * @file example_utils.h
 * @brief Shared helpers for VneScene examples: section banners and lightweight checks.
 *
 * Use @c printSection for log structure. Use @c checkNear / @c checkVec3Near / @c checkLengthNear;
 * each returns @c false on failure. Accumulate failures and return the count from @c run…Example()
 * so @c main can exit non-zero for CI smoke runs.
 */

#include "common/logging_guard.h"

#include <vertexnova/logging/logging.h>
#include <vertexnova/math/core/core.h>

#include <cmath>
#include <string>

namespace vne::scene::examples {

inline void printSection(const char* title) {
    VNE_LOG_INFO << "=== " << title << " ===";
}

inline void printSection(const std::string& title) {
    printSection(title.c_str());
}

[[nodiscard]] inline bool checkNear(const char* label, float actual, float expected, float tol) noexcept {
    const float d = std::fabs(actual - expected);
    if (d <= tol) {
        VNE_LOG_INFO << "[PASS] " << label << " (" << actual << " ~ " << expected << ")";
        return true;
    }
    VNE_LOG_ERROR << "[FAIL] " << label << " expected ~" << expected << " got " << actual << " (|delta|=" << d << ")";
    return false;
}

[[nodiscard]] inline bool checkVec3Near(const char* label,
                                        const vne::math::Vec3f& actual,
                                        const vne::math::Vec3f& expected,
                                        float tol) noexcept {
    const float dx = std::fabs(actual.x() - expected.x());
    const float dy = std::fabs(actual.y() - expected.y());
    const float dz = std::fabs(actual.z() - expected.z());
    const float m = std::max({dx, dy, dz});
    if (m <= tol) {
        VNE_LOG_INFO << "[PASS] " << label << " (max |delta|=" << m << ")";
        return true;
    }
    VNE_LOG_ERROR << "[FAIL] " << label << " expected (" << expected.x() << "," << expected.y() << "," << expected.z()
                  << ") got (" << actual.x() << "," << actual.y() << "," << actual.z() << ") max |delta|=" << m;
    return false;
}

[[nodiscard]] inline bool checkLengthNear(const char* label,
                                          const vne::math::Vec3f& v,
                                          float expected_len,
                                          float tol) noexcept {
    return checkNear(label, v.length(), expected_len, tol);
}

[[nodiscard]] inline bool checkTrue(const char* label, bool cond) noexcept {
    if (cond) {
        VNE_LOG_INFO << "[PASS] " << label;
        return true;
    }
    VNE_LOG_ERROR << "[FAIL] " << label;
    return false;
}

}  // namespace vne::scene::examples
