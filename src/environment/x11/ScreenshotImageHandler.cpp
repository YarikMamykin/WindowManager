#include "ScreenshotImageHandler.h"

#include "config/Misc.h"

#include <Imlib2.h>
#include <X11/Xutil.h>
#include <filesystem>
#include <functional>
#include <iostream>
#include <memory>

namespace {
  int get_shift_from_mask(unsigned long mask) {
    int shift = 0;
    while (!(mask & 1)) {
      mask >>= 1;
      shift++;
    }
    return shift;
  }

  void get_rgb_from_pixel(XImage* image,
                          unsigned long pixel,
                          unsigned char* r,
                          unsigned char* g,
                          unsigned char* b) {
    *r = (pixel & image->red_mask) >> get_shift_from_mask(image->red_mask);
    *g = (pixel & image->green_mask) >> get_shift_from_mask(image->green_mask);
    *b = (pixel & image->blue_mask) >> get_shift_from_mask(image->blue_mask);
  }

} // namespace

namespace ymwm::environment {
  ScreenshotImageHandler::ScreenshotImageHandler(Display* display,
                                                 int screen,
                                                 Window root_window,
                                                 int screen_width,
                                                 int screen_height) noexcept {

    std::unique_ptr<XImage, std::function<void(XImage*)>> image(
        XGetImage(display,
                  root_window,
                  0,
                  0,
                  screen_width,
                  screen_height,
                  AllPlanes,
                  ZPixmap),
        [](XImage* ptr) { XDestroyImage(ptr); });

    if (!image) {
      std::cerr << "Failed to capture screen\n";
      return;
    }

    // Initialize Imlib2
    imlib_context_set_display(display);
    imlib_context_set_visual(DefaultVisual(display, screen));
    imlib_context_set_colormap(DefaultColormap(display, screen));

    // Create an Imlib2 image from the XImage data
    Imlib_Image imlib_image = imlib_create_image(image->width, image->height);
    if (!imlib_image) {
      std::cerr << "Failed to create Imlib2 image\n";
      return;
    }

    // Set the Imlib2 image as the current context
    imlib_context_set_image(imlib_image);

    // Copy pixel data from XImage to Imlib2 image
    DATA32* imlib_data = imlib_image_get_data();
    for (int y = 0; y < image->height; ++y) {
      for (int x = 0; x < image->width; ++x) {
        unsigned long pixel = XGetPixel(image.get(), x, y);
        unsigned char r, g, b;
        get_rgb_from_pixel(image.get(), pixel, &r, &g, &b);
        imlib_data[y * image->width + x] = (r << 16) | (g << 8) | b;
      }
    }

    imlib_image_set_format("png");
    // Must be unique. Add timestamp to filename.
    auto screenshot_path =
        ymwm::config::misc::screenshots_folder / "screenshot.png";
    imlib_save_image(screenshot_path.c_str());

    // Free resources
    imlib_free_image();
  }

  ScreenshotImageHandler::~ScreenshotImageHandler() noexcept = default;
} // namespace ymwm::environment
