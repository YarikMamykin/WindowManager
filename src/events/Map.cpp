#include "Map.h"

#include "YamlModels.h"
#include "environment/Command.h"
#include "events/AbstractKeyPress.h"
#include "utils.h"

#include <iostream>
#include <variant>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/yaml.h>

namespace ymwm::events {
  Map default_event_map() noexcept {
    Map bindings;

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::e,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::ExitRequested{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::t,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::RunTerminal{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::c,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::CloseWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::j,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusNextWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::k,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusPrevWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::j,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowForward{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::k,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowBackward{});

    return bindings;
  }

  std::variant<Map, std::string>
  event_map_from_yaml(std::filesystem::path&& yamlpath) noexcept {
    Map event_map;
    try {
      YAML::Node config = YAML::LoadFile(yamlpath);
      YAML::Node key_bindings = config["key-bindings"];
      if (not key_bindings.IsSequence()) {
        std::cout << "key-bindings must be sequence\n";
        return event_map;
      }

      for (const auto& binding : key_bindings) {
        YAML::Node type = binding["type"];
        if (not type) {
          std::cout << "Type of key binding must be specified\n";
          continue;
        }

        if (auto type_value = type.as<std::string>();
            AbstractKeyPress::type == type_value) {

          auto key_press_event = binding.as<AbstractKeyPress>();

          auto cmd = utils::command_from_type(binding["cmd"].as<std::string>());
          if (not cmd) {
            return "Unknown command specified";
          }

          event_map.insert(std::make_pair(key_press_event, *cmd));
        }
      }

    } catch (const YAML::BadFile& e) {
      std::cout << e.what() << "\n";
    } catch (const YAML::Exception& e) {
      std::cout << e.what() << "\n";
    }
    return event_map.empty() ? default_event_map() : event_map;
  }
} // namespace ymwm::events
