#pragma once
#include "common/Color.h"
#include "common/Direction.h"
#include "events/AbstractKeyPress.h"
#include "utils.h"

#include <algorithm>
#include <yaml-cpp/yaml.h>

namespace YAML {
  template <>
  struct convert<ymwm::events::AbstractKeyPress> {
    static bool decode(const Node& node,
                       ymwm::events::AbstractKeyPress& event) {
      if (!node.IsMap()) {
        return false;
      }

      // Extract values from YAML node
      auto key_code = ymwm::config::utils::key_symbol_to_code(
          node["key"].as<std::string>());
      if (not key_code) {
        return false;
      }
      event.code = *key_code;

      // Mask may be unspecified
      if (YAML::Node masks = node["masks"]) {
        if (not masks.IsSequence()) {
          return false;
        }
        event.mask = ymwm::config::utils::compose_mask(masks);
      }

      return true;
    }

    static Node encode(const ymwm::events::AbstractKeyPress& event) {
      Node node;

      node["type"] = event.type;

      for (const auto& [symbol, code] :
           ymwm::config::utils::symbol_to_code_table) {
        if (code == event.code) {
          node["key"] = symbol;
          break;
        }
      }

      for (const auto& [symbol, mask] :
           ymwm::config::utils::mask_symbol_to_code_table) {
        if (event.mask & mask) {
          node["masks"].push_back(symbol);
        }
      }

      return node;
    }
  };
} // namespace YAML

namespace YAML {
  template <>
  struct convert<ymwm::common::Color> {
    static bool decode(const Node& node, ymwm::common::Color& color) {

      if (node.IsSequence()) {
        auto rgb = node.as<std::array<unsigned short, 3ul>>();
        color = ymwm::common::Color(rgb.at(0), rgb.at(1), rgb.at(2));
        return true;
      }

      if (node.IsScalar()) {
        auto rgb_in_long = node.as<unsigned long>();
        color = ymwm::common::Color(rgb_in_long);
        return true;
      }

      if (node.IsScalar()) {
        auto hex_string = node.as<std::string>();
        color = ymwm::common::Color(hex_string);
        return true;
      }

      return false;
    }

    static Node encode(const ymwm::common::Color& color) {
      Node node;
      node.push_back(color.red);
      node.push_back(color.green);
      node.push_back(color.blue);
      return node;
    }
  };

  template <>
  struct convert<ymwm::common::Direction> {
    static bool decode(const Node& node, ymwm::common::Direction& direction) {
      if (!node.IsScalar()) {
        return false;
      }

      auto value = node.as<std::string>();

      std::transform(value.begin(),
                     value.end(),
                     value.begin(),
                     [](unsigned char c) -> int { return std::tolower(c); });

      if ("up" == value) {
        direction = ymwm::common::Direction::Up;
        return true;
      }

      if ("down" == value) {
        direction = ymwm::common::Direction::Down;
        return true;
      }

      if ("left" == value) {
        direction = ymwm::common::Direction::Left;
        return true;
      }

      if ("right" == value) {
        direction = ymwm::common::Direction::Right;
        return true;
      }

      return false;
    }

    static Node encode(const ymwm::common::Direction& direction) {
      Node node;
      switch (direction) {
      case ymwm::common::Direction::Up:
        node = "Up";
        break;
      case ymwm::common::Direction::Down:
        node = "Down";
        break;
      case ymwm::common::Direction::Left:
        node = "Left";
        break;
      case ymwm::common::Direction::Right:
        node = "Right";
        break;
      }
      return node;
    }
  };
} // namespace YAML
