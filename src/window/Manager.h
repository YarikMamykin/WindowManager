#pragma once
#include "Window.h"

#include <functional>
#include <vector>

namespace ymwm::window {
  using UpdateWindowHandlerType = std::function<void(Window&)>;
  using FocusWindowHandlerType = std::function<void(Window&)>;

  struct Manager {
    Manager(UpdateWindowHandlerType&& update_window,
            FocusWindowHandlerType&& focus_window);

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;

  private:
    std::vector<Window> m_windows;
    UpdateWindowHandlerType m_update_window;
    FocusWindowHandlerType m_focus_window;
  };
} // namespace ymwm::window
