#include "environment/Environment.h"

#include "Handlers.h"
#include "environment/ColorID.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractKeyPress.h"
#include "events/Map.h"
#include "layouts/Parameters.h"

#include <array>
#include <format>
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
      : m_exit_requested(false)
      , m_manager(this) {

    // Bind error handler
    XSetErrorHandler(handleXError);
    XSetIOErrorHandler(handleXIOError);

    m_handlers = std::make_unique<Handlers>();
    m_exit_requested = not m_handlers->valid();
    if (m_exit_requested) {
      return;
    }

    auto [sw, sh] = screen_width_and_height();
    std::cout << std::format("SCREEN SIZE: {} {}\n", sw, sh);

    m_handlers->colors.insert({
        ColorID::Red,
        { .red = 0xddff,
               .green = 0,
               .blue = 0,
               .flags = DoRed | DoGreen | DoBlue }
    });
    m_handlers->colors.insert({
        ColorID::Green,
        { .red = 0,
                 .green = 0xddff,
                 .blue = 0,
                 .flags = DoRed | DoGreen | DoBlue }
    });
    m_handlers->colors.insert({
        ColorID::Blue,
        { .red = 0,
                .green = 0,
                .blue = 0xddff,
                .flags = DoRed | DoGreen | DoBlue }
    });

    for (ColorID c : { ColorID::Red, ColorID::Green, ColorID::Blue }) {
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

    layouts::Layout l;
    auto [screen_width, screen_height] = screen_width_and_height();
    l.basic_parameters.screen_width = screen_width;
    l.basic_parameters.screen_height = screen_height;
    l.basic_parameters.screen_margins.left = 10u;
    l.basic_parameters.screen_margins.right = 10u;
    l.basic_parameters.screen_margins.top = 10u;
    l.basic_parameters.screen_margins.bottom = 10u;
    l.parameters = layouts::GridParameters(
        layouts::GridParameters::Margins{ .horizontal = 10u, .vertical = 20u },
        4ul);
    m_manager.set_layout(l);
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
    case MapRequest: {
      XWindowAttributes wa;
      auto w = event.xmaprequest.window;
      if (XGetWindowAttributes(m_handlers->display, w, &wa)) {
        m_manager.add_window({ .id = w,
                               .x = wa.x,
                               .y = wa.y,
                               .w = wa.width,
                               .h = wa.height,
                               .border_width = 5,
                               .border_color = ColorID::Red });
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
