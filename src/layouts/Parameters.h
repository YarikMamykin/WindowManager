#pragma once
#include "config/Layout.h"

#include <ranges>
#include <variant>

namespace ymwm::layouts {

  struct MaximisedParameters {};

  struct GridParameters {
    config::layouts::grid::Margins margins;
    unsigned int grid_size;
    unsigned int number_of_margins;

    inline GridParameters(config::layouts::grid::Margins margins,
                          std::size_t number_of_windows)
        : margins(margins) {
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
  };

  using Parameters = std::variant<MaximisedParameters, GridParameters>;

} // namespace ymwm::layouts
