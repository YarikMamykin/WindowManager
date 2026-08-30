#pragma once
#include <array>
#include <filesystem>
#include <optional>
#include <vector>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment {

  struct ScreenshotHandler {
    using ScreenshotData = std::vector<unsigned char>;

    ScreenshotHandler& add(const std::array<int, 2ul>& coords) noexcept;
    void make(Environment& env) noexcept;
    bool has_screenshot() const noexcept;

    const std::filesystem::path& screenshot_path() const noexcept;

    void reset() noexcept;

    ScreenshotData from_file() const noexcept;

  private:
    std::optional<std::array<int, 2ul>> m_start_coords;
    std::optional<std::array<int, 2ul>> m_end_coords;
    std::filesystem::path m_screenshot_path;
  };
} // namespace ymwm::environment
