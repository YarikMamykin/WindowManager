#include "StackHorizontalDouble.h"

#include "Layout.h"
#include "config/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  StackHorizontalDouble::StackHorizontalDouble() noexcept = default;

  StackHorizontalDouble::StackHorizontalDouble(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      std::size_t number_of_windows) noexcept {
    namespace cfg = ymwm::config::layouts::stack_horizontal;

    number_of_stack_windows = number_of_windows - 1ul;
    number_of_windows_per_stack =
        number_of_stack_windows / 2 +
        static_cast<std::size_t>(0 != (number_of_stack_windows % 2));
    last_iteration = number_of_windows - 1ul;
    before_last_iteration = last_iteration - 1ul;

    int height_without_margins =
        screen_height - screen_margins.top - screen_margins.bottom;

    width_without_margins =
        screen_width - screen_margins.left - screen_margins.right;

    int height_without_margins_borders =
        screen_height - screen_margins.top - screen_margins.bottom -
        (2 * cfg::stack_window_margin) - (3 * two_borders);

    stack_window_h = (height_without_margins_borders *
                      (100 - cfg::main_window_ratio) / 100) /
                     2;

    int stack_windows_width_without_margins_and_borders =
        width_without_margins - (number_of_windows_per_stack * two_borders) -
        (cfg::stack_window_margin * (number_of_windows_per_stack - 1ul));

    stack_window_w = number_of_stack_windows > 0ul
                         ? stack_windows_width_without_margins_and_borders /
                               number_of_windows_per_stack
                         : 0;

    main_window_w = width_without_margins - two_borders;
    main_window_h =
        number_of_stack_windows > 0ul
            ? height_without_margins_borders * cfg::main_window_ratio / 100
            : (screen_height - screen_margins.top - screen_margins.bottom -
               two_borders);
    main_window_y =
        screen_margins.top +
        (number_of_stack_windows > 0ul
             ? two_borders + cfg::main_window_margin + stack_window_h
             : 0);
  }

  template <>
  void Layout::apply(const StackHorizontalDouble& parameters,
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
                 width_without_margins,
                 stack_window_w,
                 stack_window_h,
                 main_window_w,
                 main_window_h,
                 main_window_y] = parameters;

    namespace cfg = ymwm::config::layouts::stack_vertical;

    if (0ul == iteration) {
      w.x = screen_margins.left;
      w.y = main_window_y;
      w.w = main_window_w;
      w.h = main_window_h;
      return;
    }

    auto q = (iteration - 1ul) % 2ul ? 1ul : 0ul;
    w.y = screen_margins.top +
          q * (stack_window_h + two_borders + (2 * cfg::main_window_margin) +
               two_borders + main_window_h);
    w.x = screen_margins.left +
          ((iteration - 1ul) / 2) *
              (two_borders + cfg::stack_window_margin + stack_window_w);
    w.w = stack_window_w;
    w.h = stack_window_h;

    if (iteration == last_iteration or iteration == before_last_iteration) {
      // Add missing units to height of last window so it lines up equally
      // with main window. Units can be missing because of integer division.
      auto margins =
          cfg::stack_window_margin * (number_of_windows_per_stack - 1ul);
      auto borders_and_widths =
          (two_borders + stack_window_w) * number_of_windows_per_stack;
      w.w += width_without_margins - margins - borders_and_widths;
    }
  }

  template <>
  void Layout::update(const StackHorizontalDouble& parameters) noexcept {
    this->parameters =
        layouts::StackHorizontalDouble(basic_parameters.screen_margins,
                                       basic_parameters.screen_width,
                                       basic_parameters.screen_height,
                                       basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
