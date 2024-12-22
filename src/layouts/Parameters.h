#pragma once
#include <ranges>
#include <variant>

namespace ymwm::layouts {

  struct MaximisedParameters {};

  struct GridParameters {
    struct Margins {
      unsigned int horizontal{ 0u };
      unsigned int vertical{ 0u };
    } margins;
    unsigned int grid_size;
    unsigned int number_of_margins;

    inline GridParameters(Margins margins, std::size_t number_of_windows)
        : margins(margins) {
      // For now Grid layout uses symmetric grid,
      // meaning 2x2, 3x3 and so on.
      for (auto n : std::ranges::views::iota(2, 11)) {
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
