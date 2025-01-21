#pragma once
#include "Grid.h"
#include "Maximized.h"
#include "ParallelHorizontal.h"
#include "ParallelVertical.h"
#include "StackHorizontalBottom.h"
#include "StackHorizontalTop.h"
#include "StackVerticalLeft.h"
#include "StackVerticalRight.h"

#include <array>
#include <optional>
#include <string_view>
#include <variant>

namespace ymwm::layouts {

  using Parameters = std::variant<Maximised,
                                  Grid,
                                  StackVerticalRight,
                                  StackVerticalLeft,
                                  StackHorizontalTop,
                                  StackHorizontalBottom,
                                  ParallelVertical,
                                  ParallelHorizontal>;

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
