#pragma once

#include "XHeaders.h"

namespace ymwm::environment {
  struct BackgroundImageHandler {
    BackgroundImageHandler(Display* display,
                           int screen,
                           Window root_window,
                           int screen_width,
                           int screen_height) noexcept;
    ~BackgroundImageHandler() noexcept;

  private:
    Pixmap pixmap;
    GC gc;
    Display* display;
  };
} // namespace ymwm::environment
