#pragma once

namespace ymwm::events {
  struct AbstractMouseCode {
    using Type = const unsigned int;
    static Type Unknown{ 0u };

    static Type Left;
    static Type Right;
    static Type Wheel;
    static Type WheelUp;
    static Type WheelDown;
  };
} // namespace ymwm::events
