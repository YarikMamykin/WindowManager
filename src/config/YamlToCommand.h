#pragma once
#include "environment/Command.h"

#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  struct YamlToCommand {

    static std::optional<ymwm::environment::commands::Command>
    convert(const YAML::Node& cmd_node);

    struct FillCmdArgs {
      const YAML::Node& cmd_args;

      template <typename CommandType>
      void operator()(CommandType& cmd) const {
        return;
      }
    };

  private:
    static std::optional<ymwm::environment::commands::Command>
    default_command_from_node(const YAML::Node& cmd_args);
  };
} // namespace ymwm::config
