#include "AtomID.h"
#include "Handlers.h"
#include "config/Window.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractUnknownEvent.h"
#include "events/Event.h"

namespace ymwm::environment {
  std::u8string get_window_name(ymwm::environment::Handlers& handlers,
                                Window w) noexcept;

  ymwm::events::Event key_press(XEvent& event, Handlers& handlers);
  ymwm::events::Event property_notify(XEvent& event, Handlers& handlers);
  ymwm::events::Event enter_notify(XEvent& event, Handlers& handlers);
  ymwm::events::Event map_notify(XEvent& event, Handlers& handlers);
  ymwm::events::Event unmap_notify(XEvent& event, Handlers& handlers);
  ymwm::events::Event button_press(XEvent& event, Handlers& handlers);
} // namespace ymwm::environment

namespace ymwm::environment {

  ymwm::events::Event x11_to_abstract_event(XEvent& event, Handlers& handlers) {
    switch (event.type) {
    case KeyPress:
      return key_press(event, handlers);
    case PropertyNotify:
      return property_notify(event, handlers);
    case EnterNotify:
      return enter_notify(event, handlers);
    case MapRequest:
      return map_notify(event, handlers);
    case UnmapNotify:
      return unmap_notify(event, handlers);
    case ButtonPress:
      return button_press(event, handlers);
    }

    return ymwm::events::AbstractUnknownEvent{};
  }
} // namespace ymwm::environment

namespace ymwm::environment {
  ymwm::events::Event key_press(XEvent& event, Handlers& handlers) {
    return events::AbstractKeyPress{
      .code = static_cast<ymwm::events::AbstractKeyCode::Type>(
          XLookupKeysym(&event.xkey, 0)),
      .mask = static_cast<ymwm::events::AbstractKeyCode::Type>(event.xkey.state)
    };
  }

  ymwm::events::Event property_notify(XEvent& event, Handlers& handlers) {
    const auto property_atom_changed = event.xproperty.atom;
    Window w = event.xproperty.window;
    if ((XA_WM_NAME == property_atom_changed or
         handlers.atoms.at(AtomID::NetWMName) == property_atom_changed) and
        event.xproperty.state != PropertyDelete) {
      return events::WindowNameUpdated{ .wid = w,
                                        .wname = get_window_name(handlers, w) };
    }

    return events::AbstractUnknownEvent{};
  }

  ymwm::events::Event enter_notify(XEvent& event, Handlers& handlers) {

    if (NotifyNormal == event.xcrossing.mode and
        NotifyInferior != event.xcrossing.detail) {
      return events::MouseOverWindow{ .wid = event.xcrossing.window };
    }

    return events::AbstractUnknownEvent{};
  }

  ymwm::events::Event map_notify(XEvent& event, Handlers& handlers) {
    XWindowAttributes wa;
    auto w = event.xmaprequest.window;
    if (XGetWindowAttributes(handlers.display, w, &wa)) {
      // Add input mask to track property changes.
      XSelectInput(handlers.display,
                   w,
                   EnterWindowMask | FocusChangeMask | PropertyChangeMask |
                       StructureNotifyMask);
      return events::WindowAdded{
        .w = { .id = w,
              .x = wa.x,
              .y = wa.y,
              .w = wa.width,
              .h = wa.height,
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = ymwm::config::windows::regular_border_color,
              .name = get_window_name(handlers, w) }
      };
    }

    return events::AbstractUnknownEvent{};
  }

  ymwm::events::Event unmap_notify(XEvent& event, Handlers& handlers) {
    auto unmapped_window = event.xunmap.window;
    return events::WindowRemoved{ .wid = unmapped_window };
  }

  ymwm::events::Event button_press(XEvent& event, Handlers& handlers) {
    return events::AbstractMousePress{
      .mask =
          static_cast<ymwm::events::AbstractKeyMask::Type>(event.xbutton.state),
      .mcode = static_cast<ymwm::events::AbstractMousePress::Type>(
          event.xbutton.button),
      .coords = { event.xbutton.x_root, event.xbutton.y_root }
    };
  }
} // namespace ymwm::environment
