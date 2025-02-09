#pragma once
#include "environment/Command.h"
#include "environment/Environment.h"

namespace ymwm::rules {
  bool passes_rules(const ymwm::environment::commands::Command& cmd,
                    const ymwm::environment::Environment& env) noexcept;
} // namespace ymwm::rules
