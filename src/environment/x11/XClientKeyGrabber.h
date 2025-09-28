#pragma once
#include "environment/x11/Handlers.h"
#include "events/AbstractKeyPress.h"
#include "events/AbstractMousePress.h"

namespace ymwm::environment {
  struct XClientKeyGrabber {
    Handlers* handlers;

    template <typename EventType>
    inline void operator()(const EventType& event) {}

    template <>
    inline void operator()(const events::AbstractKeyPress& event) {
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
