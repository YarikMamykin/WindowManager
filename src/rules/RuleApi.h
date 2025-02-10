#pragma once
#include "environment/Command.h"
#include "environment/Environment.h"
#include "events/Event.h"

namespace ymwm::rules {
  bool passes_rules(const ymwm::environment::commands::Command& cmd,
                    [[maybe_unused]] const ymwm::events::Event& event,
                    const ymwm::environment::Environment& env) noexcept;
} // namespace ymwm::rules
