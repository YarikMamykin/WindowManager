#include "CommandToYaml.h"

#include "YamlModels.h"
#include "environment/Command.h"

#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::RunTerminal& command) const noexcept {
    auto node = default_command_node(command);
    node["args"]["path"] = command.path;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::ChangeBorderColor& command) const noexcept {
    auto node = default_command_node(command);
    node["args"]["color"] = command.color;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::MoveWindowRight& command) const noexcept {
    auto node = default_command_node(command);
    node["args"]["dx"] = command.dx;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::SetLayout& command) const noexcept {
    auto node = default_command_node(command);
    node["args"]["layout"] = command.layout;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::IncreaseMainWindowRatio& command)
      const noexcept {
    auto node = default_command_node(command);
    node["args"]["diff"] = command.diff;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::DecreaseMainWindowRatio& command)
      const noexcept {
    auto node = default_command_node(command);
    node["args"]["diff"] = command.diff;
    return node;
  }

  template <>
  YAML::Node CommandToYaml::operator()(
      const environment::commands::MoveFocusOnGrid& command) const noexcept {
    auto node = default_command_node(command);
    node["args"]["direction"] = command.direction;
    return node;
  }

} // namespace ymwm::config
