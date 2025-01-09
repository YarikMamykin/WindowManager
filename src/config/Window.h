#pragma once

#include "common/Color.h"

namespace ymwm::config::windows {
  inline ymwm::common::Color regular_border_color{ 0xff, 0x0, 0x0 };
  inline ymwm::common::Color focused_border_color{ 0x0, 0xff, 0x0 };
  constinit inline int regular_border_width{ 2 };
  constinit inline int focused_border_width{ 5 };
} // namespace ymwm::config::windows
