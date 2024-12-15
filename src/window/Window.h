#pragma once

#include "environment/ColorID.h"
#include "environment/ID.h"

namespace ymwm::window {
  struct Window {
    environment::ID id;
    int x;
    int y;
    int w;
    int h;
    int border_width;
    environment::ColorID border_color;
  };
} // namespace ymwm::window
