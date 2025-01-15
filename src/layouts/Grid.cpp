#include "Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const Grid& parameters, window::Window& w) noexcept {
    auto& [screen_width, screen_height, screen_margins, number_of_windows] =
        basic_parameters;

    auto& [margins, grid_size, number_of_margins, two_borders] = parameters;

    w.w = (screen_width - screen_margins.left - screen_margins.right -
           (margins.horizontal * number_of_margins)) /
              parameters.grid_size -
          two_borders;

    w.h = (screen_height - screen_margins.top - screen_margins.bottom -
           (parameters.margins.vertical * parameters.number_of_margins)) /
              parameters.grid_size -
          two_borders;

    auto row_index = iteration % parameters.grid_size;
    w.x = screen_margins.left +
          ((w.w + parameters.margins.horizontal + two_borders) * row_index);

    auto col_index = iteration / parameters.grid_size;
    w.y = screen_margins.top +
          ((w.h + parameters.margins.vertical + two_borders) * col_index);
  }

  template <>
  void Layout::update(const Grid& parameters) noexcept {
    this->parameters = layouts::Grid(this->basic_parameters.number_of_windows);
  }
} // namespace ymwm::layouts
