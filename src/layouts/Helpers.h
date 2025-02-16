#pragma once
#include "Parameters.h"
#include "common/TryFind.h"

#include <array>
#include <optional>
#include <string_view>

namespace ymwm::layouts {
  static inline std::optional<Parameters>
  try_find_parameters(std::string_view parameters_type) noexcept {
    return common::try_find<Parameters>(parameters_type);
  }

  template <std::size_t Index = std::variant_size_v<Parameters> - 1ul>
  static inline constexpr std::string_view
  validate(std::string_view parameters_type) noexcept {
    if constexpr (0ul <= Index) {
      using ParameterType = std::variant_alternative_t<Index, Parameters>;
      if (parameters_type == ParameterType::type) {
        return ParameterType::type;
      }

      if constexpr (0ul == Index) {
        return std::string_view{};
      } else {
        return validate<Index - 1ul>(parameters_type);
      }

    } else {
      return std::string_view{};
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

  template <typename T>
    requires requires(T t) {
      Parameters{ t };
      not std::is_same_v<Parameters, T>;
    }
  static inline constexpr bool is(const Parameters& parameters) noexcept {
    return std::holds_alternative<T>(parameters);
  }

  static inline constexpr bool
  is_stack_horizontal(const Parameters& parameters) noexcept {
    return is<StackHorizontalBottom>(parameters) or
           is<StackHorizontalTop>(parameters) or
           is<StackHorizontalDouble>(parameters);
  }

  static inline constexpr bool
  is_stack_vertical(const Parameters& parameters) noexcept {
    return is<StackVerticalLeft>(parameters) or
           is<StackVerticalRight>(parameters) or
           is<StackVerticalDouble>(parameters);
  }

  static inline constexpr bool is_stack(const Parameters& parameters) noexcept {
    return is_stack_horizontal(parameters) or is_stack_vertical(parameters);
  }
} // namespace ymwm::layouts
