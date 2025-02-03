#pragma once
// clang-format off
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <algorithm>
#include <array>
#include <cstring>
#include <cstdlib>
#include <unordered_map>
#include "common/Color.h"
#include "environment/x11/AtomID.h"
#include "config/Misc.h"
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
    std::unordered_map<common::Color, XColor, common::ColorHash> colors;
    std::array<Atom, 2ul> atoms;
    int current_layout;
    int max_layouts;

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
      current_layout = 0;
      max_layouts = get_number_of_layouts();
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

  private:
    inline int get_number_of_layouts() const noexcept {
      char* tmp = NULL;
      XkbRF_VarDefsRec vdr;
      memset(&vdr, 0, sizeof(vdr));
      XkbRF_GetNamesProp(display, &tmp, &vdr);
      int count = std::count(config::misc::language_layout.begin(),
                             config::misc::language_layout.end(),
                             ',');
      std::free(vdr.layout);
      vdr.layout = const_cast<char*>(config::misc::language_layout.data());
      count = 0 == count ? 1 : count + 1;

      std::string_view rules_filepath{ "/usr/share/X11/xkb/rules/evdev" };
      std::string_view locale{ "C" };
      XkbRF_RulesPtr rules =
          XkbRF_Load(const_cast<char*>(rules_filepath.data()),
                     const_cast<char*>(locale.data()),
                     true,
                     true);
      XkbComponentNamesRec rnames;
      XkbRF_GetComponents(rules, &vdr, &rnames);
      XkbComponentNamesRec cmdNames = { .keymap = rnames.keymap,
                                        .keycodes = rnames.keycodes,
                                        .types = rnames.types,
                                        .compat = rnames.compat,
                                        .symbols = rnames.symbols,
                                        .geometry = rnames.geometry };
      auto xkb = XkbGetKeyboardByName(display,
                                      XkbUseCoreKbd,
                                      &cmdNames,
                                      XkbGBN_AllComponentsMask,
                                      XkbGBN_AllComponentsMask &
                                          (~XkbGBN_GeometryMask),
                                      true);
      XkbRF_SetNamesProp(display, "evdev", &vdr);

      if (tmp) {
        std::free(tmp);
      }
      if (vdr.model) {
        std::free(vdr.model);
      }
      if (vdr.variant) {
        std::free(vdr.variant);
      }
      if (vdr.options) {
        std::free(vdr.options);
      }

      return count;
    }
  };

} // namespace ymwm::environment
