#pragma once
#include "events/AbstractKeyPress.h"

#include <optional>
#include <yaml-cpp/yaml.h>

namespace ymwm::config {

  struct EventToYaml {
    inline std::optional<YAML::Node>
    operator()(const auto& event) const noexcept {
      return std::nullopt;
    }

    inline std::optional<YAML::Node>
    operator()(const ymwm::events::AbstractKeyPress& event) const noexcept {
      YAML::Node node{ event };
      return node;
    }
  };
} // namespace ymwm::config
