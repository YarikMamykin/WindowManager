#pragma once

namespace ymwm::events {
  struct AbstractUnknownEvent {
    constexpr bool
    operator==(const AbstractUnknownEvent&) const noexcept = default;
    static inline constexpr char type[] = "unknown";
  };
} // namespace ymwm::events
