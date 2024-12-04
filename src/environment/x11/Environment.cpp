#include "environment/Environment.h"

#include "Handlers.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractKeyPress.h"
#include "events/Map.h"

#include <array>
#include <iostream>
#include <memory>
#include <variant>

namespace {
  // Custom error handler function
  int handleXError(Display* display, XErrorEvent* error) {
    std::array<char, 256> errorText;

    // Get the error message
    XGetErrorText(
        display, error->error_code, errorText.data(), errorText.size());

    std::cerr << "X Error: " << errorText.data() << '\n'
              << "  Major opcode: " << error->request_code << '\n'
              << "  Minor opcode: " << error->minor_code << '\n'
              << "  Resource ID: " << error->resourceid << '\n';

    // Return 0 to prevent the default error handler from being called
    return 0;
  }

  // Custom IO error handler
  int handleXIOError(Display* display) {
    std::cerr << "Fatal X11 IO Error: Connection to X server lost\n";
    exit(1);
  }

  template <class... Ts>
  struct combined_visitor : Ts... {
    using Ts::operator()...;
  };
  template <class... Ts>
  combined_visitor(Ts...) -> combined_visitor<Ts...>;
} // namespace

namespace ymwm::environment {
  Environment::Environment(const events::Map& events_map)
      : m_exit_requested(false) {

    // Bind error handler
    XSetErrorHandler(handleXError);
    XSetIOErrorHandler(handleXIOError);

    m_handlers = std::make_unique<Handlers>();
    m_exit_requested = not m_handlers->valid();
    if (m_exit_requested) {
      return;
    }

    // Grab keys by events
    XUngrabKey(
        m_handlers->display, AnyKey, AnyModifier, m_handlers->root_window);
    auto visitor = combined_visitor{
      [handlers =
           m_handlers.get()](const events::AbstractKeyPress& event) -> void {
        std::cout << "Grabbing keys: " << std::hex << event.code << " "
                  << event.mask << "\n";
        XGrabKey(handlers->display,
                 XKeysymToKeycode(handlers->display, event.code),
                 event.mask,
                 handlers->root_window,
                 true,
                 GrabModeAsync,
                 GrabModeAsync);
      },
      [](const auto& event) {
        std::cerr << "Unexpected event type: " << event.type << "\n";
      },
    };
    for (const auto& [event, _] : events_map) {
      std::visit(visitor, event);
    }

    XSetWindowAttributes wa;
    wa.cursor = m_handlers->cursor;
    wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
                    ButtonPressMask | KeyPressMask | PointerMotionMask |
                    EnterWindowMask | LeaveWindowMask | StructureNotifyMask |
                    PropertyChangeMask;
    XChangeWindowAttributes(m_handlers->display,
                            m_handlers->root_window,
                            CWEventMask | CWCursor,
                            &wa);
    XSelectInput(m_handlers->display, m_handlers->root_window, wa.event_mask);
    XSetInputFocus(m_handlers->display,
                   m_handlers->root_window,
                   RevertToPointerRoot,
                   CurrentTime);
  }

  Environment::~Environment() {
    XUngrabKey(
        m_handlers->display, AnyKey, AnyModifier, m_handlers->root_window);
  }

  events::Event Environment::event() const noexcept {
    XEvent event;
    XSync(m_handlers->display, false);
    XNextEvent(m_handlers->display, &event);
    switch (event.type) {
    case KeyPress:
      return events::AbstractKeyPress{
        .code = static_cast<ymwm::events::AbstractKeyCode::Type>(
            XLookupKeysym(&event.xkey, 0)),
        .mask =
            static_cast<ymwm::events::AbstractKeyCode::Type>(event.xkey.state)
      };
    }

    return ymwm::events::AbstractKeyPress{
      .code = ymwm::events::AbstractKeyCode::Unknown,
      .mask = ymwm::events::AbstractKeyMask::NONE
    };
  }

  Handlers& Environment::handlers() noexcept { return *m_handlers; }
} // namespace ymwm::environment
