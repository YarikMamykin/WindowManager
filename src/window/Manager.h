#pragma once
#include "Window.h"
#include "environment/ColorID.h"

#include <functional>
#include <vector>

namespace ymwm::window {
  using UpdateWindowHandlerType = std::function<void(Window&)>;
  using FocusWindowHandlerType = std::function<void(Window&)>;
  using ResetFocusHandlerType = std::function<void(void)>;
  using ChangeWindowBorderColorHandlerType = std::function<void(Window&)>;
  using MoveAndResizeWindowHandlerType = std::function<void(Window&)>;

  struct Manager {
    Manager(UpdateWindowHandlerType&& update_window,
            FocusWindowHandlerType&& focus_window,
            ResetFocusHandlerType&& reset_focus,
            ChangeWindowBorderColorHandlerType&& change_border_color,
            MoveAndResizeWindowHandlerType&& move_and_resize);

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;
    void change_border_color(environment::ColorID color) noexcept;

  private:
    std::vector<Window> m_windows;
    UpdateWindowHandlerType m_update_window;
    FocusWindowHandlerType m_focus_window;
    ResetFocusHandlerType m_reset_focus;
    ChangeWindowBorderColorHandlerType m_change_border_color;
    MoveAndResizeWindowHandlerType m_move_and_resize;
  };
} // namespace ymwm::window
