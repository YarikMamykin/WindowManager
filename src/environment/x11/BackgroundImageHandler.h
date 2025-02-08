#pragma once

#include "XHeaders.h"

#include <Imlib2.h>
namespace ymwm::environment {
  struct BackgroundImageHandler {
    BackgroundImageHandler(Display* display,
                           int screen,
                           Window root_window,
                           int screen_width,
                           int screen_height) noexcept;
    ~BackgroundImageHandler() noexcept;

  private:
    Imlib_Image image{ nullptr };
    Pixmap pixmap;
    GC gc;
    Display* display;
  };
} // namespace ymwm::environment
