
#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {
  struct ParallelHorizontal {
    static constexpr inline std::string_view type{ "ParallelHorizontal" };

    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int screen_height_without_margins_and_borders{ 0 };
    int window_width{ 0 };
    int window_height{ 0 };
    int distance_between_y{ 0 };

    ParallelHorizontal() noexcept;
    ParallelHorizontal(config::layouts::Margin screen_margins,
                       int screen_width,
                       int screen_height,
                       std::size_t number_of_windows) noexcept;
  };
} // namespace ymwm::layouts
