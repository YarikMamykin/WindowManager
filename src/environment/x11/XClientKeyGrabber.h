#pragma once
#include "environment/x11/Handlers.h"
#include "events/AbstractKeyPress.h"
#include "events/AbstractMousePress.h"

#include <iostream>

namespace ymwm::environment {
  struct XClientKeyGrabber {
    Handlers* handlers;

    template <typename EventType>
    inline void operator()(const EventType& event) {}

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

    template <>
    inline void operator()(const events::AbstractMousePress& event) {
      std::cout << "Grabbing buttons: " << std::hex << event.mcode << " "
                << event.mask << "\n";

      XGrabButton(handlers->display,
                  event.mcode,
                  event.mask,
                  handlers->root_window,
                  false,
                  ButtonPressMask | ButtonReleaseMask,
                  GrabModeAsync,
                  GrabModeSync,
                  None,
                  None);
    }
  };
} // namespace ymwm::environment
