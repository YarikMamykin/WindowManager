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

  static inline void update_main_window_ratio(Parameters& parameters,
                                              int diff) noexcept {
    if (auto* p = std::get_if<StackVerticalLeft>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<StackVerticalRight>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<StackVerticalDouble>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<StackHorizontalTop>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<StackHorizontalBottom>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<StackHorizontalDouble>(&parameters)) {
      p->main_window_ratio = p->main_window_ratio + diff;
      return;
    }

    if (auto* p = std::get_if<Centered>(&parameters)) {
      p->window_width_ratio = p->window_width_ratio + diff;
    }
  }
} // namespace ymwm::layouts
