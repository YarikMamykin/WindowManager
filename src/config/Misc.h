#pragma once

#include <filesystem>
#include <string>

namespace ymwm::config::misc {
  constinit inline std::string language_layout{ "us,ru" };
  inline std::filesystem::path background_image_path{};
  inline std::filesystem::path screenshots_folder{
    std::filesystem::temp_directory_path()
  };
} // namespace ymwm::config::misc
