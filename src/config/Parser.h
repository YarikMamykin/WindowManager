#pragma once

#include "events/Map.h"

#include <filesystem>
#include <list>
#include <stdexcept>
#include <string_view>

namespace YAML {
  struct Node;
}

namespace ymwm::config {

  struct Parser {

    Parser(std::filesystem::path&& config_path);

    [[nodiscard]] events::Map event_map() const;
    [[nodiscard]] std::list<events::Event> events_removed() const;

  private:
    events::Map event_map_from_yaml(const YAML::Node& key_bindings);
    void parse_layouts_config(const YAML::Node& layouts) const;
    void parse_window_config(const YAML::Node& window) const;

    events::Map m_event_map;
    std::list<events::Event> m_events_removed;
  };

  struct ParsingError : std::runtime_error {
    ParsingError(std::string_view err)
        : std::runtime_error(err.data()) {}
  };

} // namespace ymwm::config
