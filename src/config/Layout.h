#pragma once

#include "common/Ratio.h"
namespace ymwm::config::layouts {
  using MarginType = unsigned int;

  struct Margin {
    MarginType left;
    MarginType right;
    MarginType top;
    MarginType bottom;
  };

  constinit inline Margin screen_margins{ .left = 0u,
                                          .right = 0u,
                                          .top = 0u,
                                          .bottom = 0u };

  namespace grid {
    struct Margins {
      MarginType horizontal;
      MarginType vertical;
    };

    constinit inline Margins grid_margins{ .horizontal = 0u, .vertical = 0u };
  } // namespace grid

  namespace stack_vertical {
    using MainWindowRatioType = common::Ratio<10u, 90u>;
    inline MainWindowRatioType main_window_ratio{ 50 };
    constinit inline MarginType main_window_margin{ 5 };
    constinit inline MarginType stack_window_margin{ 5 };
  } // namespace stack_vertical
} // namespace ymwm::config::layouts
