#pragma once

#include "window/Window.h"

namespace ymwm::events {
  struct WindowAdded {
    window::Window w;

    constexpr bool operator==(const WindowAdded&) const noexcept {
      return true;
    }
    static inline constexpr std::string_view type{ "WindowAdded" };
  };
} // namespace ymwm::events
