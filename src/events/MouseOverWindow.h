#pragma once

#include "environment/ID.h"

#include <string>
namespace ymwm::events {
  struct MouseOverWindow {
    environment::ID wid;

    constexpr bool operator==(const MouseOverWindow&) const noexcept {
      return true;
    }
    static inline constexpr std::string_view type{ "MouseOverWindow" };
  };
} // namespace ymwm::events
