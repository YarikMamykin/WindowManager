#include "Centered.h"

#include "Layout.h"
#include "config/Layout.h"
#include "window/Window.h"

namespace ymwm::layouts {
  Centered::Centered() noexcept = default;
  Centered::Centered(
      config::layouts::Margin screen_margins,
      int screen_width,
      int screen_height,
      ymwm::config::layouts::centered::WindowWidthRatioType ratio) noexcept {
    window_width_ratio = ratio;

    int width_without_margins =
        screen_width - screen_margins.left - screen_margins.right;

    window_w = width_without_margins * window_width_ratio / 100 - two_borders;

    window_h = screen_height - screen_margins.top - screen_margins.bottom -
               two_borders;
    window_x = screen_margins.left * (window_width_ratio / 100) +
               width_without_margins * ((100 - window_width_ratio) / 2) / 100;
    window_y = screen_margins.top;
  }

  template <>
  void Layout::apply(const Centered& parameters, window::Window& w) noexcept {
    const auto& [screen_width, screen_height, screen_margins, _] =
        basic_parameters;

    const auto& [_,
                 window_w,
                 window_h,
                 window_x,
                 window_y,
                 window_width_ratio] = parameters;

    w.w = window_w;
    w.h = window_h;
    w.x = window_x;
    w.y = window_y;
  }

  template <>
  void Layout::update(const Centered& parameters) noexcept {
    this->parameters = Centered(this->basic_parameters.screen_margins,
                                this->basic_parameters.screen_width,
                                this->basic_parameters.screen_height,
                                parameters.window_width_ratio);
  }
} // namespace ymwm::layouts
