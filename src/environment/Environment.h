#pragma once

#include "Command.h"
#include "events/Event.h"
#include "events/Map.h"

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

  private:
    std::unique_ptr<Handlers> m_handlers;
    bool m_exit_requested;
  };
} // namespace ymwm::environment
