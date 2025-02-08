#pragma once
// clang-format off
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <array>
#include <cstring>
#include <unordered_map>
#include "common/Color.h"
#include <X11/XKBlib.h>
#include <X11/extensions/XKB.h>
#include <X11/extensions/XKBrules.h>
// clang-format on

namespace ymwm::environment {
  struct Handlers {
    Display* display;
    int screen;
    Window root_window;
    Cursor cursor;
    Colormap colormap;
    Pixmap pixmap;
    GC gc;
    std::unordered_map<common::Color, XColor, common::ColorHash> colors;
    std::array<Atom, 2ul> atoms;
    int current_layout;
    int max_layouts;

    Handlers();

    ~Handlers();

    inline bool valid() const noexcept { return static_cast<bool>(display); }

  private:
    int get_number_of_layouts() const noexcept;
    void set_background_image() noexcept;
  };

} // namespace ymwm::environment
