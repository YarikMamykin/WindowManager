#include "Grid.h"

#include "Layout.h"
#include "window/Window.h"

#include <ranges>

namespace ymwm::layouts {

  Grid::Grid() noexcept = default;

  Grid::Grid(std::size_t number_of_windows)
      : margins(config::layouts::grid::grid_margins) {
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
