#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {
  struct StackVerticalDouble {
    static constexpr inline std::string_view type{ "StackVerticalDouble" };

    std::size_t number_of_stack_windows;
    std::size_t number_of_windows_per_stack;
    std::size_t last_iteration;
    std::size_t before_last_iteration;

    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int height_without_margins;
    int stack_window_w;
    int stack_window_h;
    int main_window_w;
    int main_window_h;
    int main_window_x;
    ymwm::config::layouts::stack_vertical::MainWindowRatioType
        main_window_ratio{
          ymwm::config::layouts::stack_vertical::main_window_ratio
        };

    StackVerticalDouble() noexcept;
    StackVerticalDouble(
        config::layouts::Margin screen_margins,
        int screen_width,
        int screen_height,
        std::size_t number_of_windows,
        ymwm::config::layouts::stack_vertical::MainWindowRatioType
            ratio) noexcept;
  };
} // namespace ymwm::layouts
