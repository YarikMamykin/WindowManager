#include "BackgroundImageHandler.h"

#include "config/Misc.h"

#include <Imlib2.h>
#include <filesystem>

namespace ymwm::environment {
  BackgroundImageHandler::BackgroundImageHandler(Display* display,
                                                 int screen,
                                                 Window root_window,
                                                 int screen_width,
                                                 int screen_height) noexcept
      : display(display) {
    if (!std::filesystem::exists(config::misc::background_image_path)) {
      return;
    }

    imlib_set_cache_size(screen_width * screen_height);
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display, screen));
    imlib_context_set_colormap(DefaultColormap(display, screen));

    Imlib_Image initial_image =
        imlib_load_image(config::misc::background_image_path.c_str());

    imlib_context_set_image(initial_image);

    auto img_width = imlib_image_get_width();
    auto img_height = imlib_image_get_height();

    Imlib_Image image = imlib_create_cropped_scaled_image(
        0, 0, img_width, img_height, screen_width, screen_height);
    imlib_free_image();

    imlib_context_set_image(image);

    pixmap = XCreatePixmap(display,
                           root_window,
                           img_width,
                           img_height,
                           DefaultDepth(display, screen));
    imlib_context_set_drawable(pixmap);
    imlib_render_image_on_drawable(0, 0);
    imlib_free_image();

    // Create a graphics context
    gc = XCreateGC(display, root_window, 0, nullptr);

    // Set the pixmap as the background of the root window
    XSetWindowBackgroundPixmap(display, root_window, pixmap);

    // Clear the window to make the background take effect
    XClearWindow(display, root_window);

    // Flush the X buffer to make sure the changes take effect
    XFlush(display);
  }

  BackgroundImageHandler::~BackgroundImageHandler() noexcept {
    XFreePixmap(display, pixmap);
    XFreeGC(display, gc);
  }
} // namespace ymwm::environment
