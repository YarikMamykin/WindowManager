#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <vector>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment {

  struct ScreenshotHandler {

    ScreenshotHandler& add(const std::array<int, 2ul>& coords) noexcept;
    void make(Environment& env) noexcept;
    bool has_screenshot() const noexcept;

    const std::vector<std::uint32_t>& screenshot() const noexcept;
    const std::filesystem::path& screenshot_path() const noexcept;

    void reset() noexcept;

  private:
    std::optional<std::array<int, 2ul>> m_start_coords;
    std::optional<std::array<int, 2ul>> m_end_coords;
    std::vector<std::uint32_t> m_screenshot;
    std::filesystem::path m_screenshot_path;
  };
} // namespace ymwm::environment
