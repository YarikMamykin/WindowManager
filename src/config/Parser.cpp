#include "Parser.h"

#include "CommandToYaml.h"
#include "EventToYaml.h"
#include "YamlModels.h"
#include "common/Color.h"
#include "config/Layout.h"
#include "config/Misc.h"
#include "config/Window.h"
#include "config/YamlToCommand.h"
#include "environment/Command.h"
#include "events/Map.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
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

    if (auto background_image_path = misc["background_image"]) {
      config::misc::background_image_path =
          background_image_path.as<std::string>();
    }

    if (auto screenshots_folder = misc["screenshots_folder"]) {
      config::misc::screenshots_folder = screenshots_folder.as<std::string>();
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

    if (auto stack_vertical = layouts["stack-vertical"]) {

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

    if (auto stack_horizontal = layouts["stack-horizontal"]) {

      if (auto main_window_ratio = stack_horizontal["main-window-ratio"]) {
        ymwm::config::layouts::stack_horizontal::main_window_ratio =
            ymwm::config::layouts::stack_horizontal::MainWindowRatioType(
                main_window_ratio.as<unsigned int>());
      }

      if (auto main_window_margin = stack_horizontal["main-window-margin"]) {
        ymwm::config::layouts::stack_horizontal::main_window_margin =
            main_window_margin.as<MarginType>();
      }

      if (auto stack_window_margin = stack_horizontal["stack-window-margin"]) {
        ymwm::config::layouts::stack_horizontal::stack_window_margin =
            stack_window_margin.as<MarginType>();
      }
    }

    if (auto parallel = layouts["parallel"]) {
      if (auto margin = parallel["margin"]) {
        ymwm::config::layouts::parallel::margin =
            margin.as<layouts::MarginType>();
      }
    }

    if (auto centered = layouts["centered"]) {
      if (auto window_width_ratio = centered["window-width-ratio"]) {
        ymwm::config::layouts::centered::window_width_ratio =
            window_width_ratio
                .as<layouts::centered::WindowWidthRatioType::UnderlyingType>();
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

        auto cmd_node = binding["cmd"];
        if (not cmd_node) {
          throw ParsingError("Command is not specified for key binding");
        }

        auto cmd = YamlToCommand::convert(cmd_node);
        if (not cmd) {
          throw ParsingError("Unknown command specified");
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

  void Parser::default_config_to_yaml(std::filesystem::path&& config_path) {
    using namespace config::layouts;
    using namespace config::windows;
    using namespace config::misc;

    std::ofstream fout(config_path);
    if (not fout.is_open()) {
      throw CannotOpenFileError{ config_path.string() };
    }

    auto event_map = ymwm::events::default_event_map();

    YAML::Node config;

    // Create the "layouts" node
    YAML::Node layouts;
    layouts["screen-margins"]["left"] = screen_margins.left;
    layouts["screen-margins"]["right"] = screen_margins.right;
    layouts["screen-margins"]["top"] = screen_margins.top;
    layouts["screen-margins"]["bottom"] = screen_margins.bottom;

    layouts["grid"]["margins"]["horizontal"] = grid::grid_margins.horizontal;
    layouts["grid"]["margins"]["vertical"] = grid::grid_margins.vertical;

    layouts["stack-vertical-right"]["main-window-ratio"] =
        static_cast<stack_vertical::MainWindowRatioType::UnderlyingType>(
            stack_vertical::main_window_ratio);
    layouts["stack-vertical-right"]["main-window-margin"] =
        static_cast<stack_vertical::MainWindowRatioType::UnderlyingType>(
            stack_vertical::main_window_margin);
    layouts["stack-vertical-right"]["stack-window-margin"] =
        stack_vertical::stack_window_margin;

    layouts["stack-horizontal-right"]["main-window-ratio"] =
        static_cast<stack_horizontal::MainWindowRatioType::UnderlyingType>(
            stack_horizontal::main_window_ratio);
    layouts["stack-horizontal-right"]["main-window-margin"] =
        static_cast<stack_horizontal::MainWindowRatioType::UnderlyingType>(
            stack_horizontal::main_window_margin);
    layouts["stack-horizontal-right"]["stack-window-margin"] =
        stack_horizontal::stack_window_margin;

    layouts["parallel"]["margin"] = parallel::margin;
    layouts["centered"]["window-width-ratio"] =
        static_cast<centered::WindowWidthRatioType::UnderlyingType>(
            centered::window_width_ratio);

    // Create the "windows" node
    YAML::Node windows;
    windows["regular"]["border"]["width"] = regular_border_width;
    windows["regular"]["border"]["color"] = regular_border_color;

    windows["focused"]["border"]["width"] = focused_border_width;
    windows["focused"]["border"]["color"] = focused_border_color;

    // Create the "key-bindings" node
    YAML::Node key_bindings;

    for (const auto& [event, command] : event_map) {

      if (auto event_node = std::visit(EventToYaml{}, event)) {
        YAML::Node binding;
        binding = event_node.value();
        binding["cmd"] = std::visit(CommandToYaml{}, command);
        key_bindings.push_back(binding);
      }
    }

    YAML::Node misc;
    misc["languages"] = misc::language_layout;
    misc["background_image"] = misc::background_image_path.string();
    misc["screenshots_folder"] = misc::screenshots_folder.string();

    config["key-bindings"] = key_bindings;
    config["layouts"] = layouts;
    config["windows"] = windows;
    config["misc"] = misc;

    fout << config;
    fout.close();
  }
} // namespace ymwm::config
