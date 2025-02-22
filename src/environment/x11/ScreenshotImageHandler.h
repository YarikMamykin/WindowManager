#pragma once

#include "XHeaders.h"

namespace ymwm::environment {
  struct ScreenshotImageHandler {
    ScreenshotImageHandler(Display* display,
                           int screen,
                           Window root_window,
                           int screen_width,
                           int screen_height) noexcept;
    ~ScreenshotImageHandler() noexcept;
  };
} // namespace ymwm::environment
