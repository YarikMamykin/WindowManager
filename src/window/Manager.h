#pragma once
#include "Window.h"
#include "environment/ColorID.h"
#include "environment/ID.h"

#include <vector>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::window {
  struct Manager {
    Manager(environment::Environment* env);

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;
    void change_border_color(environment::ColorID color) noexcept;
    void move_focused_window_to(int x, int y) noexcept;
    void close_window(environment::ID id) noexcept;

  private:
    std::vector<Window> m_windows;
    environment::Environment* const m_env;
  };
} // namespace ymwm::window
