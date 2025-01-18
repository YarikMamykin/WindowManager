#include "Layout.h"
#include "config/Layout.h"
#include "layouts/Parameters.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const StackVerticalLeft& parameters,
                     window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [screen_height_without_margins,
                 two_borders,
                 number_of_stack_windows,
                 height_of_stack_window,
                 width_of_stack_window,
                 last_iteration,
                 main_window_width,
                 main_window_height] = parameters;

    namespace cfg = ymwm::config::layouts::stack_vertical_right;

    if (0ul == iteration) {
      w.x = screen_margins.left + two_borders + cfg::main_window_margin +
            width_of_stack_window;
      w.y = screen_margins.top;
      w.w = main_window_width;
      w.h = main_window_height;
      return;
    }

    w.x = screen_margins.left;
    w.y = screen_margins.top +
          (iteration - 1ul) *
              (height_of_stack_window + cfg::stack_window_margin + two_borders);
    w.w = width_of_stack_window;
    w.h = height_of_stack_window;

    if (iteration == last_iteration) {
      // Add missing units to height of last window so it lines up equally
      // with main window. Units can be missing because of integer division.
      w.h += screen_height_without_margins -
             (height_of_stack_window * number_of_stack_windows +
              two_borders * number_of_stack_windows +
              cfg::stack_window_margin * (number_of_stack_windows - 1ul));
    }
  }

  template <>
  void Layout::update(const StackVerticalLeft& parameters) noexcept {
    this->parameters =
        layouts::StackVerticalLeft(basic_parameters.screen_margins,
                                   basic_parameters.screen_width,
                                   basic_parameters.screen_height,
                                   basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
