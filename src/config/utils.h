#pragma once
#include "common/Color.h"
#include "common/VariantInterfaceHelpers.h"
#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractMouseCode.h"

#include <cctype>
#include <optional>
#include <string>
#include <unordered_map>
#include <yaml-cpp/yaml.h>

namespace ymwm::config::utils {
  inline const std::unordered_map<std::string, unsigned int>
      symbol_to_code_table{
        {         "A",           events::AbstractKeyCode::A },
        {         "B",           events::AbstractKeyCode::B },
        {         "C",           events::AbstractKeyCode::C },
        {         "D",           events::AbstractKeyCode::D },
        {         "E",           events::AbstractKeyCode::E },
        {         "F",           events::AbstractKeyCode::F },
        {         "G",           events::AbstractKeyCode::G },
        {         "H",           events::AbstractKeyCode::H },
        {         "I",           events::AbstractKeyCode::I },
        {         "J",           events::AbstractKeyCode::J },
        {         "K",           events::AbstractKeyCode::K },
        {         "L",           events::AbstractKeyCode::L },
        {         "M",           events::AbstractKeyCode::M },
        {         "N",           events::AbstractKeyCode::N },
        {         "O",           events::AbstractKeyCode::O },
        {         "P",           events::AbstractKeyCode::P },
        {         "Q",           events::AbstractKeyCode::Q },
        {         "R",           events::AbstractKeyCode::R },
        {         "S",           events::AbstractKeyCode::S },
        {         "T",           events::AbstractKeyCode::T },
        {         "U",           events::AbstractKeyCode::U },
        {         "V",           events::AbstractKeyCode::V },
        {         "W",           events::AbstractKeyCode::W },
        {         "X",           events::AbstractKeyCode::X },
        {         "Y",           events::AbstractKeyCode::Y },
        {         "Z",           events::AbstractKeyCode::Z },
        {     "Enter",       events::AbstractKeyCode::Enter },
        { "Backspace",   events::AbstractKeyCode::Backspace },
        {     "Space",       events::AbstractKeyCode::Space },
        {       "Del",         events::AbstractKeyCode::Del },
        {         "~",     events::AbstractKeyCode::Titulus },
        {         ".",      events::AbstractKeyCode::Period },
        {         ",",       events::AbstractKeyCode::Comma },
        {       "LMB",      events::AbstractMouseCode::Left },
        {       "RMB",     events::AbstractMouseCode::Right },
        {     "Wheel",     events::AbstractMouseCode::Wheel },
        {   "WheelUp",   events::AbstractMouseCode::WheelUp },
        { "WheelDown", events::AbstractMouseCode::WheelDown },
  };

  static inline std::optional<unsigned int>
  key_symbol_to_code(std::string&& symbol) noexcept {
    return symbol_to_code_table.contains(symbol)
               ? std::optional(symbol_to_code_table.at(symbol))
               : std::nullopt;
  }

  static inline std::optional<environment::commands::Command>
  command_from_type(std::string&& command_type) noexcept {
    return common::try_find_type<environment::commands::Command>(command_type);
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

} // namespace ymwm::config::utils
