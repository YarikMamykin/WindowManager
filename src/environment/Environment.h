#pragma once

#include "Command.h"
#include "events/Event.h"
#include "events/Map.h"
#include "window/Manager.h"

#include <memory>
#include <tuple>

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
    void reset_focus() noexcept;
    void update_window_border(const window::Window& w) noexcept;
    void change_border_color(const window::Window& w) noexcept;
    void move_and_resize(const window::Window& w) noexcept;
    void close_window(ID id) noexcept;
    ymwm::window::Manager<Environment>& manager() noexcept;
    std::tuple<int, int> screen_width_and_height() noexcept;

  private:
    std::unique_ptr<Handlers> m_handlers;
    bool m_exit_requested;
    mutable ymwm::window::Manager<Environment> m_manager;
  };
} // namespace ymwm::environment
