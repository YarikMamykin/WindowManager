#include "StackVerticalRight.h"

#include "Layout.h"
#include "config/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  StackVerticalRight::StackVerticalRight() noexcept = default;

  StackVerticalRight::StackVerticalRight(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      std::size_t number_of_windows,
      ymwm::config::layouts::stack_vertical::MainWindowRatioType
          ratio) noexcept {
    main_window_ratio = ratio;
    namespace cfg = ymwm::config::layouts::stack_vertical;
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

    unsigned int stack_window_ratio = 100u - main_window_ratio;

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
                         main_window_ratio / 100 +
                     two_borders + cfg::main_window_margin;

    main_window_width = number_of_stack_windows > 0ul
                            ? (screen_width_without_margins -
                               cfg::main_window_margin - double_two_borders) *
                                  main_window_ratio / 100
                            : (screen_width - screen_margins.left -
                               screen_margins.right - two_borders);

    main_window_height = screen_height_without_margins - two_borders;
  }
  template <>
  void Layout::apply(const StackVerticalRight& parameters,
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
                 stack_window_x,
                 main_window_width,
                 main_window_height,
                 main_window_ratio] = parameters;

    namespace cfg = ymwm::config::layouts::stack_vertical;

    if (0ul == iteration) {
      w.x = screen_margins.left;
      w.y = screen_margins.top;
      w.w = main_window_width;
      w.h = main_window_height;
      return;
    }

    w.x = stack_window_x;
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
  void Layout::update(const StackVerticalRight& parameters) noexcept {
    this->parameters =
        layouts::StackVerticalRight(basic_parameters.screen_margins,
                                    basic_parameters.screen_width,
                                    basic_parameters.screen_height,
                                    basic_parameters.number_of_windows,
                                    parameters.main_window_ratio);
  }
} // namespace ymwm::layouts
