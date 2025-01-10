#pragma once

#include "common/Ratio.h"
namespace ymwm::config::layouts {
  struct Margin {
    unsigned int left;
    unsigned int right;
    unsigned int top;
    unsigned int bottom;
  };

  constinit inline Margin screen_margins{ .left = 0u,
                                          .right = 0u,
                                          .top = 0u,
                                          .bottom = 0u };

  namespace grid {
    struct Margins {
      unsigned int horizontal;
      unsigned int vertical;
    };

    constinit inline Margins grid_margins{ .horizontal = 0u, .vertical = 0u };
  } // namespace grid

  namespace stack_vertical_right {
    inline common::Ratio main_window_ratio{ 50 };
    constinit inline unsigned int main_window_margin{ 5 };
    constinit inline unsigned int stack_window_margin{ 5 };
  } // namespace stack_vertical_right
} // namespace ymwm::config::layouts
