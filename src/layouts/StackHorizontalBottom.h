#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {

  struct StackHorizontalBottom {
    static constexpr inline std::string_view type{ "StackHorizontalBottom" };

    std::size_t last_iteration;
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    int stack_window_h{ 0 };
    int stack_window_w{ 0 };
    int main_window_width;
    int main_window_height;
    ymwm::config::layouts::stack_horizontal::MainWindowRatioType
        main_window_ratio{
          ymwm::config::layouts::stack_horizontal::main_window_ratio
        };

    StackHorizontalBottom() noexcept;
    StackHorizontalBottom(
        config::layouts::Margin screen_margins,
        int screen_width,
        int screen_height,
        std::size_t number_of_windows,
        ymwm::config::layouts::stack_horizontal::MainWindowRatioType
            ratio) noexcept;
  };
} // namespace ymwm::layouts
