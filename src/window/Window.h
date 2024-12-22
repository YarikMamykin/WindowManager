#pragma once

#include "environment/ColorID.h"
#include "environment/ID.h"

namespace ymwm::window {
  struct Window {
    environment::ID id{ 0 };
    int x{ 0 };
    int y{ 0 };
    int w{ 0 };
    int h{ 0 };
    int border_width{ 0 };
    environment::ColorID border_color{ environment::ColorID::Red };

    inline constexpr bool
    operator<=>(const Window& other) const noexcept = default;
  };
} // namespace ymwm::window
