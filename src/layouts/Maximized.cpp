#include "Maximized.h"

#include "Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const Maximised& parameters, window::Window& w) noexcept {
    const auto& [screen_width,
                 screen_height,
                 screen_margins,
                 number_of_windows] = basic_parameters;

    const auto& [two_borders] = parameters;

    w.w =
        screen_width - screen_margins.left - screen_margins.right - two_borders;
    w.h = screen_height - screen_margins.top - screen_margins.bottom -
          two_borders;
    w.x = screen_margins.left;
    w.y = screen_margins.top;
  }

  template <>
  void Layout::update(const Maximised& parameters) noexcept {
    this->parameters = parameters;
  }
} // namespace ymwm::layouts
