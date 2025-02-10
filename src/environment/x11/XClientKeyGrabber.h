#pragma once
#include "environment/x11/Handlers.h"
#include "events/AbstractKeyPress.h"

#include <iostream>

namespace ymwm::environment {
  struct XClientKeyGrabber {
    Handlers* handlers;

    template <typename EventType>
    inline void operator()(const EventType& event) {
      std::cerr << "Expecting AbstractKeyPress event type, not " << event.type
                << "\n";
    }

    template <>
    inline void operator()(const events::AbstractKeyPress& event) {
      std::cout << "Grabbing keys: " << std::hex << event.code << " "
                << event.mask << "\n";
      XGrabKey(handlers->display,
               XKeysymToKeycode(handlers->display, event.code),
               event.mask,
               handlers->root_window,
               true,
               GrabModeAsync,
               GrabModeAsync);
    }
  };
} // namespace ymwm::environment
