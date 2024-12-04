#pragma once
// clang-format off
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
// clang-format on

namespace ymwm::environment {
  struct Handlers {
    Display* display;
    int screen;
    Window root_window;
    Cursor cursor;

    inline Handlers() {
      display = XOpenDisplay(nullptr);
      if (not display) {
        return;
      }
      screen = DefaultScreen(display);
      root_window = RootWindow(display, screen);
      cursor = XCreateFontCursor(display, XC_left_ptr);
    }

    inline ~Handlers() {
      XFreeCursor(display, cursor);
      XDestroyWindow(display, root_window);
      XSync(display, 0);
      XSetInputFocus(display, PointerRoot, RevertToPointerRoot, CurrentTime);
      XCloseDisplay(display);
    }

    inline bool valid() const noexcept { return static_cast<bool>(display); }
  };

} // namespace ymwm::environment
