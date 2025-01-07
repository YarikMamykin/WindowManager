#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <algorithm>
#include <ranges>
#include <variant>

namespace ymwm::layouts {

  struct MaximisedParameters {};

  struct GridParameters {
    config::layouts::grid::Margins margins;
    unsigned int grid_size;
    unsigned int number_of_margins;

    inline GridParameters(config::layouts::grid::Margins margins,
                          std::size_t number_of_windows)
        : margins(margins) {
      // For now Grid layout uses symmetric grid,
      // meaning 2x2, 3x3 and so on.
      for (std::size_t n : std::ranges::views::iota(2, 11)) {
        if (number_of_windows <= (n * n)) {
          grid_size = n;
          break;
        }
      }

      number_of_margins = grid_size - 1u;
    }
  };

  struct StackVerticalRight {
    int screen_height_without_margins;
    int two_borders{ std::min(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    std::size_t number_of_stack_windows;
    int height_of_stack_window{ 0 };
    int width_of_stack_window{ 0 };
    std::size_t last_iteration;
    int stack_window_x;
    int main_window_width;
    int main_window_height;

    inline StackVerticalRight(config::layouts::Margin screen_margins,
                              int screen_width,
                              int screen_height,
                              std::size_t number_of_windows) {
      namespace cfg = ymwm::config::layouts::stack_vertical_right;
      int double_two_borders{ 2 * two_borders };

      last_iteration = number_of_windows - 1ul;

      number_of_stack_windows = number_of_windows - 1ul;

      int screen_width_without_margins =
          screen_width - screen_margins.left - screen_margins.right;
      screen_height_without_margins =
          screen_height - screen_margins.top - screen_margins.bottom;

      unsigned int height_without_margins =
          screen_height_without_margins -
          (cfg::stack_window_margin * (number_of_stack_windows - 1ul));

      unsigned int stack_window_ratio = 100u - cfg::main_window_ratio;

      if (number_of_windows > 1ul) {
        height_of_stack_window =
            (height_without_margins - (two_borders * number_of_stack_windows)) /
            number_of_stack_windows;

        width_of_stack_window = (screen_width_without_margins -
                                 cfg::main_window_margin - double_two_borders) *
                                stack_window_ratio / 100;
      }

      stack_window_x = screen_margins.left +
                       (screen_width_without_margins - cfg::main_window_margin -
                        double_two_borders) *
                           cfg::main_window_ratio / 100 +
                       two_borders + cfg::main_window_margin;

      main_window_width = (screen_width_without_margins -
                           cfg::main_window_margin - double_two_borders) *
                          cfg::main_window_ratio / 100;

      main_window_height = screen_height_without_margins - two_borders;
    }
  };

  using Parameters =
      std::variant<MaximisedParameters, GridParameters, StackVerticalRight>;

} // namespace ymwm::layouts
