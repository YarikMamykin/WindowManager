#include "AbstractKeyPress.h"
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
