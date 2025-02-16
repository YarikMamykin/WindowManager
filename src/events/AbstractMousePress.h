#pragma once
#include <string_view>

namespace ymwm::events {
  struct AbstractMousePress {
    unsigned int mask{ 0u };
    unsigned int mcode{ 0u };

    constexpr bool
    operator==(const AbstractMousePress&) const noexcept = default;
    static inline constexpr std::string_view type{ "mousepress" };
  };
} // namespace ymwm::events
