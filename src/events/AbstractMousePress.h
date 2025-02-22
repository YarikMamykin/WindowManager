#pragma once
#include <array>
#include <string_view>

namespace ymwm::events {
  struct AbstractMousePress {
    using Type = unsigned int;

    Type mask{ 0u };
    Type mcode{ 0u };
    std::array<int, 2ul> coords;

    constexpr bool operator==(const AbstractMousePress& other) const noexcept {
      return mask == other.mask and mcode == other.mcode;
    }
    static inline constexpr std::string_view type{ "mousepress" };
  };
} // namespace ymwm::events
