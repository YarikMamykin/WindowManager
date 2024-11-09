#pragma once



#include <cstdint>
namespace ymwm::events {
  struct AbstractKeyPress {
    unsigned int code{0u};
    unsigned int mask{0u};

    constexpr bool operator==(const AbstractKeyPress&) const noexcept = default;
  };
} // namespace ymwm::events
