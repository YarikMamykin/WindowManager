#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {
  struct StackHorizontalDouble {
    static constexpr inline std::string_view type{ "StackHorizontalDouble" };

    std::size_t number_of_stack_windows;
    std::size_t number_of_windows_per_stack;
    std::size_t last_iteration;
    std::size_t before_last_iteration;

    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int width_without_margins;
    int stack_window_w;
    int stack_window_h;
    int main_window_w;
    int main_window_h;
    int main_window_y;

    StackHorizontalDouble() noexcept;
    StackHorizontalDouble(config::layouts::Margin screen_margins,
                          int screen_width,
                          int screen_height,
                          std::size_t number_of_windows) noexcept;
  };
} // namespace ymwm::layouts
