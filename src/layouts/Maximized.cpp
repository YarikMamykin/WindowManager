#include "Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  template <>
  void Layout::apply(const MaximisedParameters& parameters,
                     window::Window& w) noexcept {
    auto& [screen_width,
           screen_height,
           screen_margins,
           focused_window_index,
           number_of_windows] = basic_parameters;

    w.w = screen_width - screen_margins.left - screen_margins.right;
    w.h = screen_height - screen_margins.top - screen_margins.bottom;
    w.x = screen_margins.left;
    w.y = screen_margins.top;
  }
} // namespace ymwm::layouts