#pragma once

#include "Command.h"
#include "events/Event.h"
#include "events/Map.h"
#include "window/GroupManager.h"

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
    void execute(const commands::Command& cmd,
                 [[maybe_unused]] const events::Event& event);
    bool exit_requested() const noexcept;
    void request_exit() noexcept;
    Handlers& handlers() noexcept;
    void update_window(const window::Window& w) noexcept;
    void focus_window(const window::Window& w) noexcept;
    void reset_focus() noexcept;
    void update_window_border(const window::Window& w) noexcept;
    void change_border_color(const window::Window& w) noexcept;
    void move_and_resize(const window::Window& w) noexcept;
    void close_window(const window::Window& w) noexcept;
    ymwm::window::Manager<Environment>& manager() noexcept;
    ymwm::window::GroupManager<Environment>& group() noexcept;
    std::tuple<int, int> screen_width_and_height() noexcept;
    void next_keyboard_layout() noexcept;

  private:
    std::unique_ptr<Handlers> m_handlers;
    bool m_exit_requested;
    ymwm::window::GroupManager<Environment> m_group_manager;
  };
} // namespace ymwm::environment
