#pragma once
#include "Command.h"
#include "common/TryFind.h"

namespace ymwm::environment::commands {
  static inline std::optional<environment::commands::Command>
  try_find_command(std::string_view command_type) noexcept {
    return common::try_find<environment::commands::Command>(command_type);
  }

} // namespace ymwm::environment::commands
