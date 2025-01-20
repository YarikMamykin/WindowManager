#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {

  struct StackHorizontalTop {
    static constexpr inline std::string_view type{ "StackHorizontalTop" };

    std::size_t last_iteration;
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int stack_window_h{ 0 };
    int stack_window_w{ 0 };
    int stack_window_y{ 0 };
    int main_window_width;
    int main_window_height;

    StackHorizontalTop() noexcept;
    StackHorizontalTop(config::layouts::Margin screen_margins,
                       int screen_width,
                       int screen_height,
                       std::size_t number_of_windows) noexcept;
  };
} // namespace ymwm::layouts
