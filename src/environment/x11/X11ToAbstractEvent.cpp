#include "AtomID.h"
#include "Handlers.h"
#include "config/Window.h"
#include "environment/Environment.h"
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
  ymwm::events::Event
  selection_request(XEvent& event, Handlers& handlers, Environment& e);
  ymwm::events::Event
  selection_clear(XEvent& event, Handlers& handlers, Environment& e);
} // namespace ymwm::environment

namespace ymwm::environment {

  ymwm::events::Event
  x11_to_abstract_event(XEvent& event, Handlers& handlers, Environment& e) {
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
    case SelectionRequest:
      return selection_request(event, handlers, e);
    case SelectionClear:
      return selection_clear(event, handlers, e);
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

  ymwm::events::Event
  selection_request(XEvent& event, Handlers& handlers, Environment& e) {
    XSelectionRequestEvent* req = &event.xselectionrequest;
    XSelectionEvent notify = { 0 };

    notify.type = SelectionNotify;
    notify.display = req->display;
    notify.requestor = req->requestor;
    notify.selection = req->selection;
    notify.target = req->target;
    notify.time = req->time;
    notify.property = req->property;

    if (req->selection == handlers.atoms.at(AtomID::Clipboard)) {
      if (req->target == handlers.atoms.at(AtomID::Targets)) {
        // Respond with supported targets: image/png and text/uri-list
        std::array<Atom, 3ul> supported{
          handlers.atoms.at(AtomID::ScreenshotImage),
          handlers.atoms.at(AtomID::ScreenshotPathsList),
          handlers.atoms.at(AtomID::ScreenshotPath)
        };
        XChangeProperty(handlers.display,
                        req->requestor,
                        req->property,
                        XA_ATOM,
                        32,
                        PropModeReplace,
                        reinterpret_cast<unsigned char*>(supported.data()),
                        supported.size());
      } else if (req->target == handlers.atoms.at(AtomID::ScreenshotImage)) {
        // Provide the PNG data
        XChangeProperty(handlers.display,
                        req->requestor,
                        req->property,
                        handlers.atoms.at(AtomID::ScreenshotImage),
                        8,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(
                            e.screenshot().screenshot().data()),
                        e.screenshot().screenshot().size() *
                            (sizeof(std::uint32_t) / sizeof(unsigned char)));
      } else if (req->target ==
                 handlers.atoms.at(AtomID::ScreenshotPathsList)) {
        // Provide the file URL
        auto paths_list = e.screenshot().screenshot_path().string() + "\n";
        XChangeProperty(
            handlers.display,
            req->requestor,
            req->property,
            XA_STRING,
            8,
            PropModeReplace,
            reinterpret_cast<const unsigned char*>(paths_list.c_str()),
            paths_list.size());
      } else if (req->target == handlers.atoms.at(AtomID::ScreenshotPath)) {
        auto p = e.screenshot().screenshot_path().string();
        XChangeProperty(handlers.display,
                        req->requestor,
                        req->property,
                        XA_STRING,
                        8,
                        PropModeReplace,
                        reinterpret_cast<const unsigned char*>(p.c_str()),
                        p.size());
      } else {
        // Unsupported target
        notify.property = None;
      }

      // Send the notification
      XSendEvent(handlers.display, req->requestor, False, 0, (XEvent*)&notify);
      XFlush(handlers.display);
    }

    return events::AbstractUnknownEvent{};
  }

  ymwm::events::Event
  selection_clear(XEvent& event, Handlers& handlers, Environment& e) {
    e.screenshot().reset();
    return events::AbstractUnknownEvent{};
  }
} // namespace ymwm::environment
