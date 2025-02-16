#pragma once

#include "environment/Command.h"

#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  struct CommandToYaml {
    template <typename CmdType>
    YAML::Node operator()(const CmdType& command) const noexcept {
      auto node = default_command_node(command);
      serialize_args(node, command);
      return node;
    }

  private:
    inline YAML::Node default_command_node(const auto& command) const noexcept {
      YAML::Node node;
      node["name"] = command.type;
      return node;
    }

    template <typename CmdType>
    inline void serialize_args(YAML::Node& node,
                               const CmdType& command) const noexcept {}

    template <>
    inline void serialize_args(
        YAML::Node& node,
        const environment::commands::RunTerminal& command) const noexcept {
      node["args"]["path"] = command.path;
    }

    template <>
    inline void serialize_args(YAML::Node& node,
                               const environment::commands::ChangeBorderColor&
                                   command) const noexcept {
      node["args"]["color"] = command.color;
    }

    template <>
    inline void serialize_args(
        YAML::Node& node,
        const environment::commands::MoveWindowRight& command) const noexcept {
      node["args"]["dx"] = command.dx;
    }

    template <>
    inline void serialize_args(
        YAML::Node& node,
        const environment::commands::SetLayout& command) const noexcept {
      node["args"]["layout"] = command.layout;
    }

    template <>
    inline void
    serialize_args(YAML::Node& node,
                   const environment::commands::IncreaseMainWindowRatio&
                       command) const noexcept {
      node["args"]["diff"] = command.diff;
    }

    template <>
    inline void
    serialize_args(YAML::Node& node,
                   const environment::commands::DecreaseMainWindowRatio&
                       command) const noexcept {
      node["args"]["diff"] = command.diff;
    }

    template <>
    inline void serialize_args(
        YAML::Node& node,
        const environment::commands::MoveFocusOnGrid& command) const noexcept {
      node["args"]["direction"] = command.direction;
    }
  };
} // namespace ymwm::config
