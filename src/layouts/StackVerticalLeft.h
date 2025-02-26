#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {

  struct StackVerticalLeft {
    static constexpr inline std::string_view type{ "StackVerticalLeft" };

    int screen_height_without_margins;
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    std::size_t number_of_stack_windows;
    int height_of_stack_window{ 0 };
    int width_of_stack_window{ 0 };
    std::size_t last_iteration;
    int main_window_width;
    int main_window_height;
    ymwm::config::layouts::stack_vertical::MainWindowRatioType
        main_window_ratio{
          ymwm::config::layouts::stack_vertical::main_window_ratio
        };

    StackVerticalLeft() noexcept;
    StackVerticalLeft(config::layouts::Margin screen_margins,
                      int screen_width,
                      int screen_height,
                      std::size_t number_of_windows,
                      ymwm::config::layouts::stack_vertical::MainWindowRatioType
                          ratio) noexcept;
  };
} // namespace ymwm::layouts
