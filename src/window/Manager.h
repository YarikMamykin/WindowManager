#pragma once
#include "Window.h"
#include "environment/ID.h"

namespace ymwm::window {
  struct Manager {
    void add_window(ymwm::environment::ID id) noexcept;
    void remove_window(ymwm::environment::ID id) noexcept;
  };
} // namespace ymwm::window
