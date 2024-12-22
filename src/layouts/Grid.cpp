#include "Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const GridParameters& parameters,
                     window::Window& w) noexcept {
    auto& [screen_width,
           screen_height,
           screen_margins,
           focused_window_index,
           number_of_windows] = basic_parameters;

    auto& [margins, grid_size, number_of_margins] = parameters;

    w.w = (screen_width - screen_margins.left - screen_margins.right -
           (margins.horizontal * number_of_margins)) /
          parameters.grid_size;

    w.h = (screen_height - screen_margins.top - screen_margins.bottom -
           (parameters.margins.vertical * parameters.number_of_margins)) /
          parameters.grid_size;

    auto row_index = iteration % parameters.grid_size;
    w.x = screen_margins.left +
          ((w.w + parameters.margins.horizontal) * row_index);

    auto col_index = iteration / parameters.grid_size;
    w.y =
        screen_margins.top + ((w.h + parameters.margins.vertical) * col_index);
  }
} // namespace ymwm::layouts
