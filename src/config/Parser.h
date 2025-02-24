#pragma once

#include "events/Map.h"

#include <filesystem>
#include <format>
#include <stdexcept>
#include <string_view>

namespace YAML {
  struct Node;
}

namespace ymwm::config {

  struct Parser {

    Parser(std::filesystem::path&& config_path);

    [[nodiscard]] events::Map event_map() const;

    static void default_config_to_yaml(std::filesystem::path&& config_path);

  private:
    events::Map event_map_from_yaml(const YAML::Node& key_bindings);
    void parse_layouts_config(const YAML::Node& layouts) const;
    void parse_window_config(const YAML::Node& window) const;
    void parse_misc_config(const YAML::Node& misc) const;
    events::Event event_from_node(const YAML::Node& event_node) const;

    events::Map m_event_map;
  };

  struct ParsingError : std::runtime_error {
    ParsingError(std::string_view err)
        : std::runtime_error(err.data()) {}
  };

  struct CannotOpenFileError : std::runtime_error {
    CannotOpenFileError(std::string&& err)
        : std::runtime_error(std::format("Cannot open file: {}", err)) {}
  };

} // namespace ymwm::config
