#include "YamlToCommand.h"

#include "YamlModels.h"
#include "common/Color.h"
#include "common/Direction.h"
#include "common/VariantInterfaceHelpers.h"
#include "layouts/Helpers.h"
#include "layouts/Parameters.h"

namespace ymwm::config {

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::RunTerminal& command) const {
    if (auto path = cmd_args["path"]) {
      command.path = path.as<std::string>();
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::ChangeBorderColor& command) const {
    if (auto color = cmd_args["color"]) {
      command.color = color.as<ymwm::common::Color>();
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::MoveWindowRight& command) const {
    if (auto dx = cmd_args["dx"]) {
      command.dx = dx.as<int>();
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::SetLayout& command) const {
    auto layout_type = cmd_args["layout"].as<std::string>();
    if (auto validated_layout_type =
            ymwm::common::try_validate_type<ymwm::layouts::Parameters>(
                layout_type);
        validated_layout_type.has_value()) {
      command.layout = *validated_layout_type;
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::IncreaseMainWindowRatio& command) const {
    if (auto diff = cmd_args["diff"]) {
      command.diff = diff.as<int>();
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::DecreaseMainWindowRatio& command) const {
    if (auto diff = cmd_args["diff"]) {
      command.diff = diff.as<int>();
    }
  }

  template <>
  void YamlToCommand::FillCmdArgs::operator()(
      environment::commands::MoveFocus& command) const {
    if (auto direction = cmd_args["direction"]) {
      command.direction = direction.as<ymwm::common::Direction>();
    }
  }

  std::optional<ymwm::environment::commands::Command>
  YamlToCommand::default_command_from_node(const YAML::Node& cmd_node) {
    auto command_type = cmd_node["name"].as<std::string>();
    auto command =
        common::try_find_type<environment::commands::Command>(command_type);
    return command;
  }

  std::optional<ymwm::environment::commands::Command>
  YamlToCommand::convert(const YAML::Node& cmd_node) {
    auto cmd = YamlToCommand::default_command_from_node(cmd_node);
    if (cmd and cmd_node["args"]) {
      std::visit(FillCmdArgs{ cmd_node["args"] }, *cmd);
    }
    return cmd;
  }

} // namespace ymwm::config
