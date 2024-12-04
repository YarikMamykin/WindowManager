#pragma once

namespace ymwm::events {
  struct AbstractKeyMask {
    using Type = const unsigned int;
    static Type NONE{ 0u };
    static Type Ctrl;
    static Type Alt;
    static Type Shift;
  };
} // namespace ymwm::events
