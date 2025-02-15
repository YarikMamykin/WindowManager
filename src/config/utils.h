#pragma once
#include "common/Color.h"
#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"

#include <cctype>
#include <optional>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace ymwm::config::utils {
  inline const std::unordered_map<std::string, unsigned int>
      symbol_to_code_table{
        {         "A",         events::AbstractKeyCode::A },
        {         "B",         events::AbstractKeyCode::B },
        {         "C",         events::AbstractKeyCode::C },
        {         "D",         events::AbstractKeyCode::D },
        {         "E",         events::AbstractKeyCode::E },
        {         "F",         events::AbstractKeyCode::F },
        {         "G",         events::AbstractKeyCode::G },
        {         "H",         events::AbstractKeyCode::H },
        {         "I",         events::AbstractKeyCode::I },
        {         "J",         events::AbstractKeyCode::J },
        {         "K",         events::AbstractKeyCode::K },
        {         "L",         events::AbstractKeyCode::L },
        {         "M",         events::AbstractKeyCode::M },
        {         "N",         events::AbstractKeyCode::N },
        {         "O",         events::AbstractKeyCode::O },
        {         "P",         events::AbstractKeyCode::P },
        {         "Q",         events::AbstractKeyCode::Q },
        {         "R",         events::AbstractKeyCode::R },
        {         "S",         events::AbstractKeyCode::S },
        {         "T",         events::AbstractKeyCode::T },
        {         "U",         events::AbstractKeyCode::U },
        {         "V",         events::AbstractKeyCode::V },
        {         "W",         events::AbstractKeyCode::W },
        {         "X",         events::AbstractKeyCode::X },
        {         "Y",         events::AbstractKeyCode::Y },
        {         "Z",         events::AbstractKeyCode::Z },
        {     "Enter",     events::AbstractKeyCode::Enter },
        { "Backspace", events::AbstractKeyCode::Backspace },
        {     "Space",     events::AbstractKeyCode::Space },
        {         "~",   events::AbstractKeyCode::Titulus },
  };

  static inline std::optional<unsigned int>
  key_symbol_to_code(std::string&& symbol) noexcept {
    return symbol_to_code_table.contains(symbol)
               ? std::optional(symbol_to_code_table.at(symbol))
               : std::nullopt;
  }

  static inline std::optional<environment::commands::Command>
  command_from_type(std::string&& command_type) noexcept {
    return environment::commands::try_find_command(command_type);
  }

  static inline const std::unordered_map<std::string_view,
                                         events::AbstractKeyMask::Type>
      mask_symbol_to_code_table{
        {  "Ctrl",  events::AbstractKeyMask::Ctrl },
        {   "Alt",   events::AbstractKeyMask::Alt },
        { "Shift", events::AbstractKeyMask::Shift },
  };

  static inline unsigned int
  mask_symbol_to_code(std::string&& symbol) noexcept {

    return mask_symbol_to_code_table.contains(symbol)
               ? mask_symbol_to_code_table.at(symbol)
               : events::AbstractKeyMask::NONE;
  }

  static inline unsigned int compose_mask(const YAML::Node& masks) noexcept {
    unsigned int result = events::AbstractKeyMask::NONE;
    for (const auto& mask : masks) {
      result |= mask_symbol_to_code(mask.as<std::string>());
    }
    return result;
  }

  static inline void fill_cmd_args(environment::commands::RunTerminal& cmd,
                                   const YAML::Node& args) noexcept {
    if (not args.IsMap()) {
      return;
    }

    if (auto path = args["path"]) {
      cmd.path = path.as<std::string>();
    }
  }

  static inline void
  fill_cmd_args(environment::commands::ChangeBorderColor& cmd,
                const YAML::Node& args) noexcept {
    if (not args.IsMap()) {
      return;
    }

    if (auto color = args["color"]) {
      cmd.color = color.as<ymwm::common::Color>();
    }
  }

  static inline void fill_cmd_args(environment::commands::MoveWindowRight& cmd,
                                   const YAML::Node& args) noexcept {
    if (not args.IsMap()) {
      return;
    }

    if (auto dx = args["dx"]; dx.IsScalar()) {
      cmd.dx = dx.as<int>();
    }
  }

  template <typename CommandType>
    requires requires {
      not std::is_same_v<CommandType, environment::commands::Command>;
    }
  static inline void fill_cmd_args(CommandType&, const YAML::Node&) noexcept {}

  static inline void fill_cmd_args(environment::commands::Command& cmd,
                                   const YAML::Node& args) noexcept {
    std::visit([&args](auto& command) -> void { fill_cmd_args(command, args); },
               cmd);
  }

} // namespace ymwm::config::utils
