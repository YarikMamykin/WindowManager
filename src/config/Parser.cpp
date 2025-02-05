#include "Parser.h"

#include "YamlModels.h"
#include "common/Color.h"
#include "config/Layout.h"
#include "config/Misc.h"
#include "config/Window.h"
#include "environment/Command.h"
#include "events/Map.h"

#include <algorithm>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  Parser::Parser(std::filesystem::path&& config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    parse_layouts_config(config["layouts"]);
    parse_window_config(config["windows"]);
    parse_misc_config(config["misc"]);
    m_event_map = event_map_from_yaml(config["key-bindings"]);
  }

  void Parser::parse_misc_config(const YAML::Node& misc) const {
    if (not misc) {
      return;
    }

    if (auto language_layout = misc["languages"]) {
      config::misc::language_layout = language_layout.as<std::string>();
    }
  }

  void Parser::parse_window_config(const YAML::Node& windows) const {
    if (not windows) {
      return;
    }

    if (auto focused = windows["focused"]) {
      if (auto border = focused["border"]) {

        if (auto width = border["width"]) {
          using FocusedBorderWidthType =
              decltype(ymwm::config::windows::focused_border_width);
          ymwm::config::windows::focused_border_width =
              width.as<FocusedBorderWidthType>();
        }

        if (auto color = border["color"]) {
          using FocusedBorderColorType =
              decltype(ymwm::config::windows::focused_border_color);
          ymwm::config::windows::focused_border_color =
              color.as<ymwm::common::Color>();
        }
      }
    }

    if (auto regular = windows["regular"]) {
      if (auto border = regular["border"]) {

        if (auto width = border["width"]) {
          using RegularBorderWidthType =
              decltype(ymwm::config::windows::regular_border_width);
          ymwm::config::windows::regular_border_width =
              width.as<RegularBorderWidthType>();
        }

        if (auto color = border["color"]) {
          using RegularBorderColorType =
              decltype(ymwm::config::windows::regular_border_color);
          ymwm::config::windows::regular_border_color =
              color.as<ymwm::common::Color>();
        }
      }
    }
  }

  void Parser::parse_layouts_config(const YAML::Node& layouts) const {
    if (not layouts) {
      return;
    }

    using MarginType = ymwm::config::layouts::MarginType;

    if (auto screen_margins = layouts["screen-margins"]) {

      if (auto left = screen_margins["left"]) {
        ymwm::config::layouts::screen_margins.left = left.as<MarginType>();
      }

      if (auto right = screen_margins["right"]) {
        ymwm::config::layouts::screen_margins.right = right.as<MarginType>();
      }

      if (auto top = screen_margins["top"]) {
        ymwm::config::layouts::screen_margins.top = top.as<MarginType>();
      }

      if (auto bottom = screen_margins["bottom"]) {
        ymwm::config::layouts::screen_margins.bottom = bottom.as<MarginType>();
      }
    }

    if (auto grid = layouts["grid"]) {
      if (auto margins = grid["margins"]) {

        if (auto horizontal = margins["horizontal"]) {
          ymwm::config::layouts::grid::grid_margins.horizontal =
              horizontal.as<MarginType>();
        }

        if (auto vertical = margins["vertical"]) {
          ymwm::config::layouts::grid::grid_margins.vertical =
              vertical.as<MarginType>();
        }
      }
    }

    if (auto stack_vertical = layouts["stack-vertical-right"]) {

      if (auto main_window_ratio = stack_vertical["main-window-ratio"]) {
        ymwm::config::layouts::stack_vertical::main_window_ratio =
            ymwm::config::layouts::stack_vertical::MainWindowRatioType(
                main_window_ratio.as<unsigned int>());
      }

      if (auto main_window_margin = stack_vertical["main-window-margin"]) {
        ymwm::config::layouts::stack_vertical::main_window_margin =
            main_window_margin.as<MarginType>();
      }

      if (auto stack_window_margin = stack_vertical["stack-window-margin"]) {
        ymwm::config::layouts::stack_vertical::stack_window_margin =
            stack_window_margin.as<MarginType>();
      }
    }
  }

  events::Map Parser::event_map_from_yaml(const YAML::Node& key_bindings) {
    events::Map event_map = events::default_event_map();

    std::set<environment::commands::Command> cmds_created;
    for (const auto& event_to_cmd : event_map) {
      cmds_created.insert(event_to_cmd.second);
    }

    if (not key_bindings) {
      return event_map;
    }

    if (not key_bindings.IsSequence()) {
      throw ParsingError("key-bindings must be sequence\n");
    }

    for (const auto& binding : key_bindings) {
      YAML::Node type = binding["type"];
      if (not type) {
        throw ParsingError("Type of key binding must be specified\n");
      }

      if (auto type_value = type.as<std::string>();
          events::AbstractKeyPress::type == type_value) {

        ymwm::events::Event key_press_event =
            binding.as<events::AbstractKeyPress>();

        if (not binding["cmd"]) {
          throw ParsingError("Command is not specified for key binding");
        }

        auto cmd =
            utils::command_from_type(binding["cmd"]["name"].as<std::string>());
        if (not cmd) {
          throw ParsingError("Unknown command specified");
        }

        if (auto cmd_args = binding["cmd"]["args"]) {
          utils::fill_cmd_args(*cmd, cmd_args);
        }

        auto [_, no_duplicate] = cmds_created.insert(*cmd);
        event_map[key_press_event] = *cmd;

        if (no_duplicate) {
          continue;
        }

        // If duplicated command spotted,
        // remove all previous occasions except last one,
        // which should override the value.
        if (auto found_event_to_cmd = std::find_if(
                event_map.begin(),
                event_map.end(),
                [&cmd, &key_press_event](const auto& mapping) -> bool {
                  return cmd == mapping.second and
                         key_press_event != mapping.first;
                });
            event_map.end() != found_event_to_cmd) {
          m_events_removed.push_back(found_event_to_cmd->first);
          event_map.erase(found_event_to_cmd);
        }
      }
    }

    return event_map;
  }

  [[nodiscard]] events::Map Parser::event_map() const { return m_event_map; }
  [[nodiscard]] std::list<events::Event> Parser::events_removed() const {
    return m_events_removed;
  }
} // namespace ymwm::config
