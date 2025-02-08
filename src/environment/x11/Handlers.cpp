#include "Handlers.h"

#include "config/Misc.h"
#include "environment/x11/AtomID.h"
#include "environment/x11/CPtrHandler.h"
#include "environment/x11/XkbComponentNamesRecWrapper.h"
#include "environment/x11/XkbRF_VarDefsRecWrapper.h"

#include <Imlib2.h>
#include <algorithm>

namespace ymwm::environment {
  Handlers::Handlers() {
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
    set_background_image();
  }

  Handlers::~Handlers() {
    imlib_free_image();
    XFreePixmap(display, pixmap);
    XFreeGC(display, gc);
    XFreeColormap(display, colormap);
    XFreeCursor(display, cursor);
    XDestroyWindow(display, root_window);
    XSync(display, 0);
    XSetInputFocus(display, PointerRoot, RevertToPointerRoot, CurrentTime);
    XCloseDisplay(display);
  }

  int Handlers::get_number_of_layouts() const noexcept {
    CPtrWrapper<char*> tmp;
    XkbRF_VarDefsRecWrapper<XkbRF_VarDefsRec> vdr;
    memset(&vdr, 0, sizeof(vdr));
    XkbRF_GetNamesProp(display, &tmp.ptr, &vdr.v);
    std::free(vdr.v.layout);
    vdr.v.layout = const_cast<char*>(config::misc::language_layout.data());

    std::string_view rules_filepath{ "/usr/share/X11/xkb/rules/evdev" };
    std::string_view locale{ "C" };
    CPtrWrapper<XkbRF_RulesPtr> rules{
      .ptr = XkbRF_Load(const_cast<char*>(rules_filepath.data()),
                        const_cast<char*>(locale.data()),
                        true,
                        true)
    };

    XkbComponentNamesRecWrapper<XkbComponentNamesRec> rnames;
    XkbRF_GetComponents(rules.ptr, &vdr.v, &rnames.v);
    CPtrWrapper<XkbDescPtr> xkb{ .ptr = XkbGetKeyboardByName(
                                     display,
                                     XkbUseCoreKbd,
                                     &rnames.v,
                                     XkbGBN_AllComponentsMask,
                                     XkbGBN_AllComponentsMask &
                                         (~XkbGBN_GeometryMask),
                                     true) };
    std::string_view rules_filename{ "evdev" };
    XkbRF_SetNamesProp(
        display, const_cast<char*>(rules_filename.data()), &vdr.v);

    int count = std::count(config::misc::language_layout.begin(),
                           config::misc::language_layout.end(),
                           ',');
    count = 0 == count ? 1 : count + 1;
    return count;
  }

  void Handlers::set_background_image() noexcept {
    int screen_width = XDisplayWidth(display, screen);
    int screen_height = XDisplayHeight(display, screen);
    imlib_set_cache_size(screen_width * screen_height);
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display, screen));
    imlib_context_set_colormap(DefaultColormap(display, screen));

    Imlib_Image image =
        imlib_load_image(config::misc::background_image_path.c_str());

    imlib_context_set_image(image);

    auto img_width = imlib_image_get_width();
    auto img_height = imlib_image_get_height();

    auto scaled_img = imlib_create_cropped_scaled_image(
        0, 0, img_width, img_height, screen_width, screen_height);
    imlib_free_image();

    imlib_context_set_image(scaled_img);

    pixmap = XCreatePixmap(display,
                           root_window,
                           img_width,
                           img_height,
                           DefaultDepth(display, screen));
    imlib_context_set_drawable(pixmap);
    imlib_render_image_on_drawable(0, 0);

    // Create a graphics context
    gc = XCreateGC(display, root_window, 0, nullptr);

    // Set the pixmap as the background of the root window
    XSetWindowBackgroundPixmap(display, root_window, pixmap);

    // Clear the window to make the background take effect
    XClearWindow(display, root_window);

    // Flush the X buffer to make sure the changes take effect
    XFlush(display);
  }
} // namespace ymwm::environment
