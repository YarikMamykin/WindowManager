#include "ParallelHorizontal.h"

#include "layouts/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  ParallelHorizontal::ParallelHorizontal() noexcept = default;
  ParallelHorizontal::ParallelHorizontal(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      std::size_t number_of_windows) noexcept {
    screen_height_without_margins_and_borders =
        screen_height - screen_margins.top - screen_margins.bottom -
        ((number_of_windows - 1ul) * config::layouts::parallel::margin) -
        (number_of_windows * two_borders);
    window_height =
        screen_height_without_margins_and_borders / number_of_windows;
    window_width =
        screen_width - screen_margins.left - screen_margins.right - two_borders;
    distance_between_y =
        two_borders + window_height + config::layouts::parallel::margin;
  }

  template <>
  void Layout::apply(const ParallelHorizontal& parameters,
                     window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [two_borders,
                 screen_height_without_margins_and_borders,
                 window_width,
                 window_height,
                 distance_between_y] = parameters;

    w.y = screen_margins.top + iteration * distance_between_y;
    w.x = screen_margins.left;
    w.w = window_width;
    w.h = window_height;

    if (number_of_windows - 1ul == iteration) {
      w.h = w.h + screen_height_without_margins_and_borders -
            (window_height * number_of_windows);
    }
  }

  template <>
  void Layout::update(const ParallelHorizontal& parameters) noexcept {
    this->parameters =
        layouts::ParallelHorizontal(basic_parameters.screen_margins,
                                    basic_parameters.screen_width,
                                    basic_parameters.screen_height,
                                    basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
