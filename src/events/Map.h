#pragma once

#include "Event.h"
#include "Hash.h"
#include "environment/Command.h"

#include <unordered_map>

namespace ymwm::events {
  using Map =
      std::unordered_map<Event, ymwm::environment::commands::Command, Hasher>;

  Map default_event_map() noexcept;

} // namespace ymwm::events
