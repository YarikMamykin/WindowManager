#include "StackVerticalDouble.h"

#include "Layout.h"
#include "config/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  StackVerticalDouble::StackVerticalDouble() noexcept = default;

  StackVerticalDouble::StackVerticalDouble(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      std::size_t number_of_windows) noexcept {
    namespace cfg = ymwm::config::layouts::stack_vertical;

    number_of_stack_windows = number_of_windows - 1ul;
    number_of_windows_per_stack =
        number_of_stack_windows / 2 +
        static_cast<std::size_t>(0 != (number_of_stack_windows % 2));
    last_iteration = number_of_windows - 1ul;
    before_last_iteration = last_iteration - 1ul;

    height_without_margins =
        screen_height - screen_margins.top - screen_margins.bottom;

    int width_without_margins_borders =
        screen_width - screen_margins.left - screen_margins.right -
        (2 * cfg::stack_window_margin) - (3 * two_borders);

    main_window_w =
        width_without_margins_borders * cfg::main_window_ratio / 100;
    main_window_h = height_without_margins - two_borders;
    main_window_x = screen_margins.left + two_borders +
                    cfg::main_window_margin +
                    ((width_without_margins_borders - main_window_w) / 2);

    stack_window_w =
        (width_without_margins_borders * (100 - cfg::main_window_ratio) / 100) /
        2;

    int stack_windows_height_without_margins_and_borders =
        height_without_margins - (number_of_windows_per_stack * two_borders) -
        ((number_of_windows_per_stack - 1ul) * cfg::stack_window_margin);

    stack_window_h = stack_windows_height_without_margins_and_borders /
                     number_of_windows_per_stack;
  }

  template <>
  void Layout::apply(const StackVerticalDouble& parameters,
                     window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [number_of_stack_windows,
                 number_of_windows_per_stack,
                 last_iteration,
                 before_last_iteration,
                 two_borders,
                 height_without_margins,
                 stack_window_w,
                 stack_window_h,
                 main_window_w,
                 main_window_h,
                 main_window_x] = parameters;

    namespace cfg = ymwm::config::layouts::stack_vertical;

    if (0ul == iteration) {
      w.x = main_window_x;
      w.y = screen_margins.top;
      w.w = main_window_w;
      w.h = main_window_h;
      return;
    }

    auto q = (iteration - 1ul) % 2ul ? 1ul : 0ul;
    w.x = screen_margins.left +
          q * (stack_window_w + two_borders + (2 * cfg::main_window_margin) +
               two_borders + main_window_w);
    w.y = screen_margins.top +
          ((iteration - 1ul) / 2) *
              (two_borders + cfg::stack_window_margin + stack_window_h);
    w.w = stack_window_w;
    w.h = stack_window_h;

    if (iteration == last_iteration or iteration == before_last_iteration) {
      // Add missing units to height of last window so it lines up equally
      // with main window. Units can be missing because of integer division.
      auto margins =
          cfg::stack_window_margin * (number_of_windows_per_stack - 1ul);
      auto borders_and_widths =
          (two_borders + stack_window_h) * number_of_windows_per_stack;
      w.h += height_without_margins - margins - borders_and_widths;
    }
  }

  template <>
  void Layout::update(const StackVerticalDouble& parameters) noexcept {
    this->parameters =
        layouts::StackVerticalDouble(basic_parameters.screen_margins,
                                     basic_parameters.screen_width,
                                     basic_parameters.screen_height,
                                     basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
