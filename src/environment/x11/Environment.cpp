#include "environment/Environment.h"

#include "Handlers.h"
#include "XClientKeyGrabber.h"
#include "common/Color.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractKeyPress.h"
#include "layouts/Parameters.h"

#include <format>
#include <iostream>

namespace ymwm::environment {
  int handle_x_error(Display* display, XErrorEvent* error);
  int handle_x_io_error(Display* display);
} // namespace ymwm::environment

namespace ymwm::environment {
  XColor xcolor_from_color(const common::Color& c) noexcept;
  std::u8string get_window_name(ymwm::environment::Handlers& handlers,
                                Window w) noexcept;
} // namespace ymwm::environment

namespace ymwm::environment {
  Environment::Environment(const events::Map& events_map)
      : m_exit_requested(false)
      , m_manager(this) {

    // Bind error handler
    XSetErrorHandler(handle_x_error);
    XSetIOErrorHandler(handle_x_io_error);

    m_handlers = std::make_unique<Handlers>();
    m_exit_requested = not m_handlers->valid();
    if (m_exit_requested) {
      return;
    }

    auto [sw, sh] = screen_width_and_height();
    std::cout << std::format("SCREEN SIZE: {} {}\n", sw, sh);

    for (const auto& c : {
             common::Color{ 0xff,  0x0,  0x0 },
             common::Color{  0x0, 0xff,  0x0 },
             common::Color{  0x0,  0x0, 0xff }
    }) {
      m_handlers->colors.insert({ c, xcolor_from_color(c) });
      if (not XAllocColor(m_handlers->display,
                          m_handlers->colormap,
                          &m_handlers->colors.at(c))) {
        std::cerr << std::format("Failed to allocate color: {} {} {}\n",
                                 m_handlers->colors.at(c).red,
                                 m_handlers->colors.at(c).green,
                                 m_handlers->colors.at(c).blue);
        m_exit_requested = true;
        return;
      }
    }

    // Grab keys by events
    XUngrabKey(
        m_handlers->display, AnyKey, AnyModifier, m_handlers->root_window);
    XClientKeyGrabber visitor{ .handlers = m_handlers.get() };
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

    m_manager.layout().update(layouts::Margin{
        .left = 10u, .right = 10u, .top = 10u, .bottom = 10u });
    m_manager.layout().update(layouts::GridParameters(
        layouts::GridParameters::Margins{ .horizontal = 10u, .vertical = 20u },
        4ul));
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
    case PropertyNotify: {
      const auto property_atom_changed = event.xproperty.atom;
      Window w = event.xproperty.window;
      if ((XA_WM_NAME == property_atom_changed or
           m_handlers->atoms.at(AtomID::NetWMName) == property_atom_changed) and
          event.xproperty.state != PropertyDelete) {
        m_manager.update_window_name(w, get_window_name(*m_handlers, w));
      }
      break;
    }
    case MapRequest: {
      XWindowAttributes wa;
      auto w = event.xmaprequest.window;
      if (XGetWindowAttributes(m_handlers->display, w, &wa)) {
        // Add input mask to track property changes.
        XSelectInput(m_handlers->display,
                     w,
                     EnterWindowMask | FocusChangeMask | PropertyChangeMask |
                         StructureNotifyMask);
        m_manager.add_window({
            .id = w,
            .x = wa.x,
            .y = wa.y,
            .w = wa.width,
            .h = wa.height,
            .border_width = 5,
            .border_color = common::Color{ 0xff, 0x0, 0x0 },
            .name = get_window_name(*m_handlers, w)
        });
      }

      break;
    }
    case UnmapNotify: {
      auto unmapped_window = event.xunmap.window;
      m_manager.remove_window(unmapped_window);
      break;
    }
    case DestroyNotify: {
      break;
    }
    }

    return ymwm::events::AbstractKeyPress{
      .code = ymwm::events::AbstractKeyCode::Unknown,
      .mask = ymwm::events::AbstractKeyMask::NONE
    };
  }

  Handlers& Environment::handlers() noexcept { return *m_handlers; }

  void Environment::update_window(const window::Window& w) noexcept {
    XResizeWindow(m_handlers->display, w.id, w.w, w.h);
    XSetWindowBorderWidth(m_handlers->display, w.id, w.border_width);
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);
  }

  void Environment::update_window_border(const window::Window& w) noexcept {
    XSetWindowBorderWidth(m_handlers->display, w.id, w.border_width);
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);
    XEvent expose_event;
    expose_event.type = Expose;
    expose_event.xexpose.window = w.id;
    expose_event.xexpose.count = 0;
    XSendEvent(m_handlers->display, w.id, False, ExposureMask, &expose_event);
  }

  void Environment::move_and_resize(const window::Window& w) noexcept {
    XMoveResizeWindow(m_handlers->display, w.id, w.x, w.y, w.w, w.h);
  }

  void Environment::focus_window(const window::Window& w) noexcept {
    XRaiseWindow(m_handlers->display, w.id);
    XMapWindow(m_handlers->display, w.id);
    XSetInputFocus(m_handlers->display, w.id, RevertToPointerRoot, CurrentTime);
  }

  void Environment::reset_focus() noexcept {
    XSetInputFocus(m_handlers->display,
                   m_handlers->root_window,
                   RevertToPointerRoot,
                   CurrentTime);
  }

  void Environment::change_border_color(const window::Window& w) noexcept {
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);

    // Sending expose event is required for redraw!
    XEvent expose_event;
    expose_event.type = Expose;
    expose_event.xexpose.window = w.id;
    expose_event.xexpose.count = 0;
    XSendEvent(m_handlers->display, w.id, False, ExposureMask, &expose_event);
  }

  void Environment::close_window(const window::Window& w) noexcept {
    auto id = w.id;
    // XUnmapWindow(m_handlers->display, id);
    // XDestroyWindow(m_handlers->display, id);
    XKillClient(m_handlers->display, id);
  }

  std::tuple<int, int> Environment::screen_width_and_height() noexcept {
    return { XDisplayWidth(m_handlers->display, m_handlers->screen),
             XDisplayHeight(m_handlers->display, m_handlers->screen) };
  }
} // namespace ymwm::environment
