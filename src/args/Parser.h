#pragma once

#include <filesystem>

namespace ymwm::args {

  struct ParsedArgs {
    std::filesystem::path generated_default_config_path;
    std::filesystem::path config_path;
  };

  ParsedArgs parse(int argc, char** argv);
} // namespace ymwm::args
