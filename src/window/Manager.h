#pragma once
#include "Window.h"

#include <vector>

namespace ymwm::window {
  struct Manager {
    Manager();

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;

  private:
    std::vector<Window> m_windows;
  };
} // namespace ymwm::window
