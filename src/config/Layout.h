#pragma once

namespace ymwm::config::layouts {
  struct Margin {
    unsigned int left;
    unsigned int right;
    unsigned int top;
    unsigned int bottom;
  };

  namespace maximized {
    static constinit inline Margin screen_margins{ .left = 0u,
                                                   .right = 0u,
                                                   .top = 0u,
                                                   .bottom = 0u };
  }

  namespace grid {
    static constinit inline Margin screen_margins{ .left = 0u,
                                                   .right = 0u,
                                                   .top = 0u,
                                                   .bottom = 0u };

    struct Margins {
      unsigned int horizontal;
      unsigned int vertical;
    };

    static constinit inline Margins grid_margins{ .horizontal = 0u,
                                                  .vertical = 0u };
  } // namespace grid

  namespace stack_vertical_right {
    static constinit inline unsigned int main_window_ratio{ 50 };
    static constinit inline unsigned int main_window_margin{ 5 };
    static constinit inline unsigned int stack_window_margin{ 5 };
  } // namespace stack_vertical_right
} // namespace ymwm::config::layouts
