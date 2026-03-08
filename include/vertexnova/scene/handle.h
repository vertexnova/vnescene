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
 * @file handle.h
 * @brief Type-safe generational handles for scene objects (Option A).
 *
 * 32-bit slot + 32-bit generation = 64-bit; identity is uint64_t.
 * CameraId and LightId are distinct types (no passing CameraId where LightId expected).
 */

#include <cstdint>

namespace vne::scene {

/**
 * @class Handle
 * @brief Generational handle: slot (32 bits) + generation (32 bits). Max value is invalid.
 */
template<typename Tag>
class Handle {
   public:
    constexpr Handle() = default;
    explicit constexpr Handle(uint64_t v) noexcept
        : value_(v) {}

    /** @brief Invalid handle (raw max). */
    static constexpr Handle Invalid() noexcept { return Handle(UINT64_MAX); }

    /** @brief True if not Invalid(). */
    [[nodiscard]] constexpr bool IsValid() const noexcept { return value_ != UINT64_MAX; }

    /** @brief Slot index (low 32 bits). */
    [[nodiscard]] constexpr uint32_t Slot() const noexcept { return static_cast<uint32_t>(value_ & 0xFFFFFFFFu); }
    /** @brief Generation (high 32 bits). */
    [[nodiscard]] constexpr uint32_t Gen() const noexcept { return static_cast<uint32_t>(value_ >> 32); }
    /** @brief Raw 64-bit value. */
    [[nodiscard]] constexpr uint64_t Raw() const noexcept { return value_; }

    /** @brief Build from slot and generation. Reserves max for invalid (returns 0 if slot+gen would be max). */
    static constexpr Handle Make(uint32_t slot, uint32_t gen) noexcept {
        uint64_t v = (static_cast<uint64_t>(gen) << 32) | static_cast<uint64_t>(slot);
        return Handle(v == UINT64_MAX ? 0 : v);
    }

    friend constexpr bool operator==(Handle a, Handle b) noexcept { return a.value_ == b.value_; }
    friend constexpr bool operator!=(Handle a, Handle b) noexcept { return a.value_ != b.value_; }

   private:
    uint64_t value_ = UINT64_MAX;
};

/** @brief Tag for camera handles. */
struct CameraTag {};
/** @brief Tag for light handles. */
struct LightTag {};

/** @brief Type-safe camera handle. */
using CameraId = Handle<CameraTag>;
/** @brief Type-safe light handle. */
using LightId = Handle<LightTag>;

}  // namespace vne::scene
