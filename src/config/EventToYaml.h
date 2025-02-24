#pragma once
#include "events/AbstractKeyPress.h"
#include "events/AbstractMousePress.h"

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

    inline std::optional<YAML::Node>
    operator()(const ymwm::events::AbstractMousePress& event) const noexcept {
      YAML::Node node{ event };
      return node;
    }
  };
} // namespace ymwm::config
