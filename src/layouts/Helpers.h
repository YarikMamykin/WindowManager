#pragma once
#include "Parameters.h"

namespace ymwm::layouts {

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
