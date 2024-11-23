#pragma once

#include "Event.h"
#include "Hash.h"
#include "commands/Command.h"

#include <filesystem>
#include <unordered_map>
#include <variant>

namespace ymwm::events {
  using Map = std::unordered_map<Event, ymwm::commands::Command, Hasher>;

  Map default_event_map() noexcept;

  std::variant<Map, std::string>
  event_map_from_yaml(std::filesystem::path&& yamlfile) noexcept;
} // namespace ymwm::events
