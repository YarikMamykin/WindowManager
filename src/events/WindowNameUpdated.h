#pragma once

#include "environment/ID.h"

#include <string>
namespace ymwm::events {
  struct WindowNameUpdated {
    environment::ID wid;
    std::u8string wname;

    constexpr bool operator==(const WindowNameUpdated&) const noexcept {
      return true;
    }
    static inline constexpr std::string_view type{ "WindowNameUpdated" };
  };
} // namespace ymwm::events
