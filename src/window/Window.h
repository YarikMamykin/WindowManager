#pragma once

#include "environment/ID.h"

namespace ymwm::window {
  struct Window {
    environment::ID id;
    int x;
    int y;
    int w;
    int h;
  };
} // namespace ymwm::window
