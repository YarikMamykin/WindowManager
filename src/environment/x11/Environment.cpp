#include "environment/Environment.h"

#include "Handlers.h"
#include "XClientKeyGrabber.h"
#include "common/Color.h"
#include "environment/x11/AtomID.h"

namespace ymwm::environment {
  int handle_x_error(Display* display, XErrorEvent* error);
  int handle_x_io_error(Display* display);
  XColor xcolor_from_color(const common::Color& c) noexcept;
  ymwm::events::Event
  x11_to_abstract_event(XEvent& event, Handlers& handlers, Environment& e);
  bool register_colors(const std::array<common::Color, 3ul>& colors,
                       Handlers& handlers);
  void x_send_expose_event(Handlers& handlers, ID window_id) noexcept;
} // namespace ymwm::environment

namespace ymwm::environment {
  Environment::Environment(const events::Map& events_map)
      : m_exit_requested(false)
      , m_group_manager(this) {

    // Bind error handler
    XSetErrorHandler(handle_x_error);
    XSetIOErrorHandler(handle_x_io_error);

    m_handlers = std::make_unique<Handlers>();
    m_exit_requested = not m_handlers->valid();
    if (m_exit_requested) {
      return;
    }

    auto colors = std::array<common::Color, 3ul>{
      ymwm::config::windows::regular_border_color,
      ymwm::config::windows::focused_border_color,
      common::Color{ 0x0, 0x0, 0xff }
    };
    m_exit_requested = not register_colors(colors, *m_handlers);
    if (m_exit_requested) {
      return;
    }

    // Grab keys by events
    XUngrabKey(
        m_handlers->display, AnyKey, AnyModifier, m_handlers->root_window);
    XUngrabButton(
        m_handlers->display, AnyButton, AnyModifier, m_handlers->root_window);

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
  }

  Environment::~Environment() {
    XUngrabKey(
        m_handlers->display, AnyKey, AnyModifier, m_handlers->root_window);
    XUngrabButton(
        m_handlers->display, AnyButton, AnyModifier, m_handlers->root_window);
  }

  events::Event Environment::event() noexcept {
    XEvent event;
    XSync(m_handlers->display, false);
    XNextEvent(m_handlers->display, &event);
    return x11_to_abstract_event(event, *m_handlers, *this);
  }

  Handlers& Environment::handlers() noexcept { return *m_handlers; }

  void Environment::update_window(const window::Window& w) noexcept {
    XResizeWindow(m_handlers->display, w.id, w.w, w.h);
    XSetWindowBorderWidth(m_handlers->display, w.id, w.border_width);
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);
    x_send_expose_event(*m_handlers, w.id);
  }

  void Environment::update_window_border(const window::Window& w) noexcept {
    XSetWindowBorderWidth(m_handlers->display, w.id, w.border_width);
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);
    x_send_expose_event(*m_handlers, w.id);
  }

  void Environment::move_and_resize(const window::Window& w) noexcept {
    XMoveResizeWindow(m_handlers->display, w.id, w.x, w.y, w.w, w.h);
  }

  void Environment::open_window(const window::Window& w) noexcept {
    XMapWindow(m_handlers->display, w.id);
  }

  void Environment::focus_window(const window::Window& w) noexcept {
    XRaiseWindow(m_handlers->display, w.id);
    XSetInputFocus(m_handlers->display, w.id, RevertToPointerRoot, CurrentTime);
    XChangeProperty(m_handlers->display,
                    m_handlers->root_window,
                    m_handlers->atoms.at(AtomID::NetActiveWindow),
                    XA_WINDOW,
                    32,
                    PropModeReplace,
                    (unsigned char*)&(w.id),
                    1);
    x_send_expose_event(*m_handlers, w.id);
  }

  void Environment::reset_focus() noexcept {
    XSetInputFocus(m_handlers->display,
                   m_handlers->root_window,
                   RevertToPointerRoot,
                   CurrentTime);
    XDeleteProperty(m_handlers->display,
                    m_handlers->root_window,
                    m_handlers->atoms.at(AtomID::NetActiveWindow));
  }

  void Environment::change_border_color(const window::Window& w) noexcept {
    XSetWindowBorder(
        m_handlers->display, w.id, m_handlers->colors.at(w.border_color).pixel);

    // Sending expose event is required for redraw!
    x_send_expose_event(*m_handlers, w.id);
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

  void Environment::next_keyboard_layout() noexcept {
    m_handlers->current_layout++;
    if (m_handlers->max_layouts == m_handlers->current_layout) {
      m_handlers->current_layout = 0;
    }
    XkbLockGroup(
        m_handlers->display, XkbUseCoreKbd, m_handlers->current_layout);
  }
} // namespace ymwm::environment
