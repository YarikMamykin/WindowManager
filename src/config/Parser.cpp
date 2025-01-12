#include "Parser.h"

#include "YamlModels.h"
#include "common/Color.h"
#include "common/Ratio.h"
#include "config/Layout.h"
#include "config/Window.h"
#include "environment/Command.h"
#include "events/Map.h"

#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  Parser::Parser(std::filesystem::path&& config_path) {
    YAML::Node config = YAML::LoadFile(config_path);
    parse_layouts_config(config["layouts"]);
    parse_window_config(config["windows"]);
    m_event_map = event_map_from_yaml(config["key-bindings"]);
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

    if (auto stack_vertical_right = layouts["stack-vertical-right"]) {

      if (auto main_window_ratio = stack_vertical_right["main-window-ratio"]) {
        ymwm::config::layouts::stack_vertical_right::main_window_ratio =
            ymwm::common::Ratio(main_window_ratio.as<unsigned int>());
      }

      if (auto main_window_margin =
              stack_vertical_right["main-window-margin"]) {
        ymwm::config::layouts::stack_vertical_right::main_window_margin =
            main_window_margin.as<MarginType>();
      }

      if (auto stack_window_margin =
              stack_vertical_right["stack-window-margin"]) {
        ymwm::config::layouts::stack_vertical_right::stack_window_margin =
            stack_window_margin.as<MarginType>();
      }
    }
  }

  events::Map Parser::event_map_from_yaml(const YAML::Node& key_bindings) {
    std::set<environment::commands::Command> cmds_created;

    events::Map event_map = events::default_event_map();
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
        if (no_duplicate) {
          event_map.insert(std::make_pair(key_press_event, *cmd));
          continue;
        }
        m_events_removed.push_back(key_press_event);
      }
    }

    return event_map;
  }

  [[nodiscard]] events::Map Parser::event_map() const { return m_event_map; }
  [[nodiscard]] std::list<events::Event> Parser::events_removed() const {
    return m_events_removed;
  }
} // namespace ymwm::config
