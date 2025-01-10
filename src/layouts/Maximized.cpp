#include "Layout.h"
#include "layouts/Parameters.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const Maximised& parameters, window::Window& w) noexcept {
    auto& [screen_width, screen_height, screen_margins, number_of_windows] =
        basic_parameters;

    w.w = screen_width - screen_margins.left - screen_margins.right -
          (2 * w.border_width);
    w.h = screen_height - screen_margins.top - screen_margins.bottom -
          (2 * w.border_width);
    w.x = screen_margins.left;
    w.y = screen_margins.top;
  }

  template <>
  void Layout::update(const Maximised& parameters) noexcept {
    this->parameters = parameters;
  }
} // namespace ymwm::layouts
