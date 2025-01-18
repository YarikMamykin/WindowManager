#pragma once
#include "config/Layout.h"
#include "config/Window.h"

#include <string_view>

namespace ymwm::layouts {
  struct Grid {
    static constexpr inline std::string_view type{ "Grid" };

    config::layouts::grid::Margins margins;
    unsigned int grid_size;
    unsigned int number_of_margins;
    int two_borders{ std::max(config::windows::regular_border_width,
                              config::windows::focused_border_width) *
                     2 };

    Grid() noexcept;
    Grid(std::size_t number_of_windows);
  };
} // namespace ymwm::layouts
