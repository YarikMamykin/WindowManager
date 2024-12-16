#pragma once
#include "Window.h"

#include <functional>
#include <vector>

namespace ymwm::window {
  using UpdateWindowHandlerType = std::function<void(Window&)>;
  using FocusWindowHandlerType = std::function<void(Window&)>;
  using ResetFocusHandlerType = std::function<void(void)>;

  struct Manager {
    Manager(UpdateWindowHandlerType&& update_window,
            FocusWindowHandlerType&& focus_window,
            ResetFocusHandlerType&& reset_focus);

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;

  private:
    std::vector<Window> m_windows;
    UpdateWindowHandlerType m_update_window;
    FocusWindowHandlerType m_focus_window;
    ResetFocusHandlerType m_reset_focus;
  };
} // namespace ymwm::window
