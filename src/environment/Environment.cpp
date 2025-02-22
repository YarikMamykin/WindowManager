#include "Environment.h"

#include "events/Event.h"

#include <variant>

namespace ymwm::environment {

  void Environment::execute(const commands::Command& cmd,
                            const events::Event& event) {
    std::visit([env = this, &event](const auto& c) { c.execute(*env, event); },
               cmd);
  }

  bool Environment::exit_requested() const noexcept { return m_exit_requested; }

  void Environment::request_exit() noexcept { m_exit_requested = true; }

  ymwm::window::Manager<Environment>& Environment::manager() noexcept {
    return m_group_manager.manager();
  }

  ymwm::window::GroupManager<Environment>& Environment::group() noexcept {
    return m_group_manager;
  }

  ScreenshotHandler& Environment::screenshot() noexcept {
    return m_screenshot_handler;
  }
} // namespace ymwm::environment
