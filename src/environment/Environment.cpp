#include "Environment.h"

#include <variant>

namespace ymwm::environment {

  void Environment::execute(const commands::Command& cmd) {
    std::visit([env = this](const auto& c) { c.execute(*env); }, cmd);
  }

  bool Environment::exit_requested() const noexcept { return m_exit_requested; }

  void Environment::request_exit() noexcept { m_exit_requested = true; }

  ymwm::window::Manager& Environment::manager() noexcept { return m_manager; }
} // namespace ymwm::environment
