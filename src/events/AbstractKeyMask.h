#pragma once

namespace ymwm::events {
  struct AbstractKeyMask {
    static const unsigned int None{ 0u };
    static const unsigned int Ctrl;
    static const unsigned int Alt;
    static const unsigned int Shift;
  };
} // namespace ymwm::events
