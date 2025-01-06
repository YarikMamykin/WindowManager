#include "AbstractKeyPress.h"
#include "common/Color.h"
#include "utils.h"

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
      auto key_code = ymwm::events::utils::key_symbol_to_code(
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
        event.mask = ymwm::events::utils::compose_mask(masks);
      }

      return true;
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
  };
} // namespace YAML
