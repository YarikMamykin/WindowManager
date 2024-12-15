#pragma once

#include "Command.h"
#include "events/Event.h"
#include "events/Map.h"
#include "window/Manager.h"

#include <memory>

namespace ymwm::environment {
  struct Handlers;
}

namespace ymwm::environment {
  struct Environment {
    Environment(const events::Map& events_map);
    ~Environment();

    events::Event event() const noexcept;
    void execute(const commands::Command& cmd);
    bool exit_requested() const noexcept;
    void request_exit() noexcept;
    Handlers& handlers() noexcept;
    void update_window(const window::Window& w) noexcept;
    void focus_window(const window::Window& w) noexcept;

  private:
    std::unique_ptr<Handlers> m_handlers;
    bool m_exit_requested;
    mutable ymwm::window::Manager m_manager;
  };
} // namespace ymwm::environment
