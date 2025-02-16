#pragma once

#include <string_view>
namespace ymwm::events {
  struct AbstractUnknownEvent {
    constexpr bool
    operator==(const AbstractUnknownEvent&) const noexcept = default;
    static inline constexpr std::string_view type{ "unknown" };
  };
} // namespace ymwm::events
