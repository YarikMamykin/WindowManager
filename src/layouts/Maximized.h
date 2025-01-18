#pragma once

#include "config/Window.h"

namespace ymwm::layouts {
  struct Maximised {
    static constexpr inline std::string_view type{ "Maximised" };
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
  };

} // namespace ymwm::layouts
