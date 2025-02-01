#pragma once

#include "config/Layout.h"
#include "config/Window.h"

namespace ymwm::layouts {
  struct Centered {
    static constexpr inline std::string_view type{ "Centered" };
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int window_w{ 0 };
    int window_h{ 0 };
    int window_x{ 0 };
    int window_y{ 0 };

    Centered() noexcept;
    Centered(config::layouts::Margin screen_margins,
             int screen_width,
             int screen_height) noexcept;
  };

} // namespace ymwm::layouts
