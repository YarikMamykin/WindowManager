#include "environment/Command.h"
#include "environment/Environment.h"

namespace ymwm::environment::commands {
  void Dummy::execute([[maybe_unused]] Environment&) const {}
  void ExitRequested::execute(Environment& e) const { e.request_exit(); }
} // namespace ymwm::environment::commands
