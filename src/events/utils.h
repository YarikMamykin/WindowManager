#pragma once
#include "AbstractKeyCode.h"
#include "AbstractKeyMask.h"
#include "environment/Command.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace ymwm::events::utils {
  static inline std::optional<unsigned int>
  key_symbol_to_code(std::string&& symbol) noexcept {
    static const std::unordered_map<std::string, unsigned int>
        symbol_to_code_table{
          { "A", AbstractKeyCode::A },
          { "B", AbstractKeyCode::B },
    };

    return symbol_to_code_table.contains(symbol)
               ? std::optional(symbol_to_code_table.at(symbol))
               : std::nullopt;
  }

  static inline std::optional<environment::commands::Command>
  command_from_type(std::string&& command_type) noexcept {
    static const std::unordered_map<std::string_view,
                                    environment::commands::Command>
        type_to_cmd_table{
          {   environment::commands::RunTerminal::type,
           environment::commands::RunTerminal{}  },
          { environment::commands::ExitRequested::type,
           environment::commands::ExitRequested{} }
    };
    return type_to_cmd_table.contains(command_type)
               ? std::optional(type_to_cmd_table.at(command_type))
               : std::nullopt;
  }

  static inline unsigned int
  mask_symbol_to_code(std::string&& symbol) noexcept {

    static const std::unordered_map<std::string, unsigned int>
        symbol_to_code_table{
          {  "Ctrl",  AbstractKeyMask::Ctrl },
          {   "Alt",   AbstractKeyMask::Alt },
          { "Shift", AbstractKeyMask::Shift },
    };
    return symbol_to_code_table.contains(symbol)
               ? symbol_to_code_table.at(symbol)
               : AbstractKeyMask::NONE;
  }

  static inline unsigned int compose_mask(const YAML::Node& masks) noexcept {
    unsigned int result = AbstractKeyMask::NONE;
    for (const auto& mask : masks) {
      result |= mask_symbol_to_code(mask.as<std::string>());
    }
    return result;
  }

} // namespace ymwm::events::utils
