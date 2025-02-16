#pragma once
#include <string_view>

namespace ymwm::events {
  struct AbstractKeyPress {
    unsigned int code{ 0u };
    unsigned int mask{ 0u };

    constexpr bool operator==(const AbstractKeyPress&) const noexcept = default;
    static inline constexpr std::string_view type{ "press" };
  };
} // namespace ymwm::events
