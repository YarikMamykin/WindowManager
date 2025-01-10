#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <algorithm>
#include <ranges>
#include <string_view>
#include <variant>

namespace ymwm::layouts {

  struct MaximisedParameters {
    static constexpr inline std::string_view type{ "Maximised" };
  };

  struct GridParameters {
    static constexpr inline std::string_view type{ "Grid" };

    config::layouts::grid::Margins margins;
    unsigned int grid_size;
    unsigned int number_of_margins;

    inline GridParameters() noexcept = default;

    inline GridParameters(std::size_t number_of_windows)
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
  };

  struct StackVerticalRight {
    static constexpr inline std::string_view type{ "StackVerticalRight" };

    int screen_height_without_margins;
    int two_borders{ std::min(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };
    std::size_t number_of_stack_windows;
    int height_of_stack_window{ 0 };
    int width_of_stack_window{ 0 };
    std::size_t last_iteration;
    int stack_window_x;
    int main_window_width;
    int main_window_height;

    inline StackVerticalRight() noexcept = default;

    inline StackVerticalRight(config::layouts::Margin screen_margins,
                              int screen_width,
                              int screen_height,
                              std::size_t number_of_windows) {
      namespace cfg = ymwm::config::layouts::stack_vertical_right;
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

      unsigned int stack_window_ratio = 100u - cfg::main_window_ratio;

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
                           cfg::main_window_ratio / 100 +
                       two_borders + cfg::main_window_margin;

      main_window_width = (screen_width_without_margins -
                           cfg::main_window_margin - double_two_borders) *
                          cfg::main_window_ratio / 100;

      main_window_height = screen_height_without_margins - two_borders;
    }
  };

  using Parameters =
      std::variant<MaximisedParameters, GridParameters, StackVerticalRight>;

  template <std::size_t Index = std::variant_size_v<Parameters> - 1ul>
  static inline std::optional<Parameters>
  try_find_parameters(std::string_view parameters_type) noexcept {
    if constexpr (0ul <= Index) {
      using ParameterType = std::variant_alternative_t<Index, Parameters>;
      if (parameters_type == ParameterType::type) {
        return ParameterType{};
      }

      if constexpr (0ul == Index) {
        return std::nullopt;
      } else {
        return try_find_parameters<Index - 1ul>(parameters_type);
      }

    } else {
      return std::nullopt;
    }
  }

  using ListOfParametersTypes =
      std::array<std::string_view, std::variant_size_v<Parameters>>;

  template <std::size_t Index = std::variant_size_v<Parameters> - 1ul>
  static inline consteval void
  fetch_parameters_types(ListOfParametersTypes& list_of_types) noexcept {
    list_of_types.at(Index) =
        std::variant_alternative_t<Index, Parameters>::type;

    if constexpr (0ul == Index) {
      return;
    } else {
      fetch_parameters_types<Index - 1ul>(list_of_types);
    }
  }

  template <std::size_t Index = std::variant_size_v<Parameters> - 1ul>
  static inline consteval ListOfParametersTypes list_of_parameters() noexcept {
    ListOfParametersTypes list_of_parameters_types;
    fetch_parameters_types(list_of_parameters_types);
    return list_of_parameters_types;
  }
} // namespace ymwm::layouts
