#pragma once

#include "common/Color.h"
#include "environment/ID.h"

#include <string>

namespace ymwm::window {
  struct Window {
    environment::ID id{ 0 };
    int x{ 0 };
    int y{ 0 };
    int w{ 0 };
    int h{ 0 };
    int border_width{ 0 };
    common::Color border_color{ 0xff, 0x0, 0x0 };
    std::u8string name{};

    inline constexpr bool
    operator<=>(const Window& other) const noexcept = default;
  };
} // namespace ymwm::window
