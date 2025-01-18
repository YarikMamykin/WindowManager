#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {
  struct StackVerticalRight {
    static constexpr inline std::string_view type{ "StackVerticalRight" };

    int screen_height_without_margins;
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    std::size_t number_of_stack_windows;
    int height_of_stack_window{ 0 };
    int width_of_stack_window{ 0 };
    std::size_t last_iteration;
    int stack_window_x;
    int main_window_width;
    int main_window_height;

    StackVerticalRight() noexcept;
    StackVerticalRight(config::layouts::Margin screen_margins,
                       int screen_width,
                       int screen_height,
                       std::size_t number_of_windows) noexcept;
  };
} // namespace ymwm::layouts
