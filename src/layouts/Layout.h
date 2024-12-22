#pragma once
#include "Parameters.h"

#include <cstddef>
#include <functional>

namespace ymwm::window {
  struct Window;
}

namespace ymwm::layouts {

  struct Margin {
    unsigned int left{ 0u };
    unsigned int right{ 0u };
    unsigned int top{ 0u };
    unsigned int bottom{ 0u };
  };

  struct Layout {
    struct BasicParameters {
      int screen_width;
      int screen_height;
      Margin screen_margins;
      std::size_t focused_window_index;
      std::size_t number_of_windows;
    } basic_parameters;

    Parameters parameters;

    [[maybe_unused]] std::size_t iteration{ 0ul };

    template <typename ParametersType>
    void apply(const ParametersType& parameters, window::Window& w) noexcept;

    [[nodiscard]] std::function<void(window::Window&)>
    operator()(const auto& parameters) {
      return [&parameters, layout = this](window::Window& w) -> void {
        layout->apply(parameters, w);
      };
    }
  };

} // namespace ymwm::layouts
