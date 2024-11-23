#pragma once

namespace ymwm::events {
  struct AbstractMousePress {
    unsigned int mask{ 0u };
    unsigned int mcode{ 0u };

    constexpr bool
    operator==(const AbstractMousePress&) const noexcept = default;
  };
} // namespace ymwm::events
