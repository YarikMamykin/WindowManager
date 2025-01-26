#include "StackHorizontalTop.h"

#include "Layout.h"
#include "config/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  StackHorizontalTop::StackHorizontalTop() noexcept = default;

  StackHorizontalTop::StackHorizontalTop(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      std::size_t number_of_windows) noexcept {
    namespace cfg = ymwm::config::layouts::stack_horizontal;

    last_iteration = number_of_windows - 1ul;
    std::size_t number_of_stack_windows = number_of_windows - 1ul;

    int height_without_margins_and_borders =
        screen_height - screen_margins.top - screen_margins.bottom -
        cfg::main_window_margin - 2 * two_borders;
    main_window_width =
        screen_width - screen_margins.left - screen_margins.right - two_borders;
    main_window_height =
        number_of_stack_windows > 0ul
            ? height_without_margins_and_borders * cfg::main_window_ratio / 100
            : (screen_height - screen_margins.top - screen_margins.bottom -
               two_borders);

    stack_window_h = height_without_margins_and_borders *
                     (100 - cfg::main_window_ratio) / 100;
    if (number_of_stack_windows > 0ul) {
      stack_window_w =
          (screen_width - screen_margins.left - screen_margins.right -
           (number_of_windows * two_borders) -
           (number_of_stack_windows - 1ul) * cfg::stack_window_margin) /
          (number_of_stack_windows);
    }
    stack_window_y = screen_margins.top + cfg::main_window_margin +
                     two_borders + main_window_height;
  }

  template <>
  void Layout::apply(const StackHorizontalTop& parameters,
                     window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [last_iteration,
                 two_borders,
                 stack_window_h,
                 stack_window_w,
                 stack_window_y,
                 main_window_width,
                 main_window_height] = parameters;

    namespace cfg = ymwm::config::layouts::stack_horizontal;

    if (0ul == iteration) {
      w.x = screen_margins.left;
      w.y = screen_margins.top;
      w.w = main_window_width;
      w.h = main_window_height;
      return;
    }

    w.x = screen_margins.left +
          (iteration - 1ul) *
              (two_borders + cfg::stack_window_margin + stack_window_w);
    w.y = stack_window_y;
    w.w = stack_window_w;
    w.h = stack_window_h;

    if (iteration == last_iteration) {
      // Add missing units to height of last window so it lines up equally
      // with main window. Units can be missing because of integer division.
      w.w += screen_width - screen_margins.left - screen_margins.right -
             ((two_borders + stack_window_w) * (number_of_windows - 1ul) +
              cfg::stack_window_margin * (number_of_windows - 2ul));
    }
  }

  template <>
  void Layout::update(const StackHorizontalTop& parameters) noexcept {
    this->parameters =
        layouts::StackHorizontalTop(basic_parameters.screen_margins,
                                    basic_parameters.screen_width,
                                    basic_parameters.screen_height,
                                    basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
