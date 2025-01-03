#pragma once
// clang-format off
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <array>
#include <unordered_map>
#include "common/Color.h"
#include "environment/x11/AtomID.h"
// clang-format on

namespace ymwm::environment {
  struct Handlers {
    Display* display;
    int screen;
    Window root_window;
    Cursor cursor;
    Colormap colormap;
    std::unordered_map<common::Color, XColor, common::ColorHash> colors;
    std::array<Atom, 2ul> atoms;

    inline Handlers() {
      display = XOpenDisplay(nullptr);
      if (not display) {
        return;
      }
      screen = DefaultScreen(display);
      root_window = RootWindow(display, screen);
      cursor = XCreateFontCursor(display, XC_left_ptr);
      colormap = XCreateColormap(
          display, root_window, DefaultVisual(display, screen), AllocNone);
      colors.reserve(1);
      atoms.at(AtomID::NetWMName) = XInternAtom(display, "_NET_WM_NAME", False);
      atoms.at(AtomID::Utf8String) = XInternAtom(display, "UTF8_STRING", False);
    }

    inline ~Handlers() {
      XFreeColormap(display, colormap);
      XFreeCursor(display, cursor);
      XDestroyWindow(display, root_window);
      XSync(display, 0);
      XSetInputFocus(display, PointerRoot, RevertToPointerRoot, CurrentTime);
      XCloseDisplay(display);
    }

    inline bool valid() const noexcept { return static_cast<bool>(display); }
  };

} // namespace ymwm::environment
