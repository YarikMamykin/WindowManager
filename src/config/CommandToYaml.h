#pragma once

#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  struct CommandToYaml {
    template <typename CmdType>
    YAML::Node operator()(const CmdType& command) const noexcept {
      return default_command_node(command);
    }

  private:
    inline YAML::Node default_command_node(const auto& command) const noexcept {
      YAML::Node node;
      node["name"] = command.type;
      return node;
    }
  };
} // namespace ymwm::config
