#pragma once

namespace ymwm::events {
  struct AbstractKeyCode {
    using Type = const unsigned int;
    static Type Unknown{ 0u };
    static Type a;
    static Type b;
    static Type c;
    static Type A;
    static Type B;
    static Type C;
  };
} // namespace ymwm::events
