#pragma once
#include "Parameters.h"

#include <cstddef>
#include <functional>

namespace ymwm::window {
  struct Window;
}

namespace ymwm::layouts {

  struct Layout {
    struct BasicParameters {
      int screen_width;
      int screen_height;
      config::layouts::Margin screen_margins;
      std::size_t number_of_windows;
    } basic_parameters;

    Parameters parameters;

    [[maybe_unused]] std::size_t iteration{ 0ul };

    inline void apply(window::Window& w) noexcept {
      std::invoke(std::visit(*this, this->parameters), w);
    }

    template <typename ParametersType>
    void apply(const ParametersType& parameters, window::Window& w) noexcept;

    struct LayoutUpdator {
      Layout* l;
      inline void operator()(const auto& parameters) noexcept {
        l->update(parameters);
      }
    };

    inline void update(const BasicParameters& basic_parameters,
                       const Parameters& parameters) noexcept {
      iteration = 0ul;
      this->basic_parameters = basic_parameters;
      std::visit(LayoutUpdator{ .l = this }, parameters);
    }

    template <typename ParametersType>
    void update(const ParametersType& parameters) noexcept;

    [[nodiscard]] inline std::function<void(window::Window&)>
    operator()(const auto& parameters) {
      return [&parameters, layout = this](window::Window& w) -> void {
        layout->apply(parameters, w);
        ++layout->iteration;
      };
    }
  };

} // namespace ymwm::layouts
