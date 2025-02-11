#pragma once

#include "environment/ID.h"

#include <string_view>

namespace ymwm::events {
  struct WindowRemoved {
    environment::ID wid;

    constexpr bool operator==(const WindowRemoved&) const noexcept {
      return true;
    }
    static inline constexpr std::string_view type{ "WindowRemoved" };
  };
} // namespace ymwm::events
