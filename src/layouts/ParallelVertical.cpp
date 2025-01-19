#include "ParallelVertical.h"

#include "layouts/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  ParallelVertical::ParallelVertical() noexcept = default;
  ParallelVertical::ParallelVertical(config::layouts::Margin screen_margins,
                                     int screen_width,
                                     int screen_height,
                                     std::size_t number_of_windows) noexcept {
    screen_width_without_margins_and_borders =
        screen_width - screen_margins.left - screen_margins.right -
        ((number_of_windows - 1ul) * config::layouts::parallel::margin) -
        (number_of_windows * two_borders);
    window_width = screen_width_without_margins_and_borders / number_of_windows;
    window_height = screen_height - screen_margins.top - screen_margins.bottom -
                    two_borders;
    distance_between_x =
        two_borders + window_width + config::layouts::parallel::margin;
  }

  template <>
  void Layout::apply(const ParallelVertical& parameters,
                     window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [two_borders,
                 screen_width_without_margins_and_borders,
                 window_width,
                 window_height,
                 distance_between_x] = parameters;

    w.x = screen_margins.left + iteration * distance_between_x;
    w.y = screen_margins.top;
    w.w = window_width;
    w.h = window_height;

    if (number_of_windows - 1ul == iteration) {
      w.w = w.w + screen_width_without_margins_and_borders -
            (window_width * number_of_windows);
    }
  }

  template <>
  void Layout::update(const ParallelVertical& parameters) noexcept {
    this->parameters =
        layouts::ParallelVertical(basic_parameters.screen_margins,
                                  basic_parameters.screen_width,
                                  basic_parameters.screen_height,
                                  basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
