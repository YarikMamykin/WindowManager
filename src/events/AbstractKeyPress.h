#pragma once

namespace ymwm::events {
  struct AbstractKeyPress {
    unsigned int code{ 0u };
    unsigned int mask{ 0u };

    constexpr bool operator==(const AbstractKeyPress&) const noexcept = default;
    static inline constexpr char type[] = "press";
  };
} // namespace ymwm::events
