#pragma once
#include <array>
#include <optional>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment {

  struct ScreenshotHandler {

    ScreenshotHandler& add(const std::array<int, 2ul>& coords) noexcept;
    void make(Environment& env) noexcept;

  private:
    std::optional<std::array<int, 2ul>> m_start_coords;
    std::optional<std::array<int, 2ul>> m_end_coords;
  };
} // namespace ymwm::environment
