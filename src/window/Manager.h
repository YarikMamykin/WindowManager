#pragma once
#include "Window.h"
#include "environment/ColorID.h"
#include "environment/ID.h"

#include <functional>
#include <vector>

namespace ymwm::window {
  using UpdateWindowHandlerType = std::function<void(Window&)>;
  using FocusWindowHandlerType = std::function<void(Window&)>;
  using ResetFocusHandlerType = std::function<void(void)>;
  using ChangeWindowBorderColorHandlerType = std::function<void(Window&)>;
  using MoveAndResizeWindowHandlerType = std::function<void(Window&)>;
  using CloseWindowHandlerType = std::function<void(environment::ID)>;

  struct Manager {
    Manager(UpdateWindowHandlerType&& update_window,
            FocusWindowHandlerType&& focus_window,
            ResetFocusHandlerType&& reset_focus,
            ChangeWindowBorderColorHandlerType&& change_border_color,
            MoveAndResizeWindowHandlerType&& move_and_resize,
            CloseWindowHandlerType&& close_window);

    void add_window(const Window& w) noexcept;
    void remove_window(environment::ID id) noexcept;
    const std::vector<Window>& windows() const noexcept;
    bool has_window(environment::ID id) const noexcept;
    void change_border_color(environment::ColorID color) noexcept;
    void move_focused_window_to(int x, int y) noexcept;
    void close_window(environment::ID id) noexcept;

  private:
    std::vector<Window> m_windows;
    UpdateWindowHandlerType m_update_window;
    FocusWindowHandlerType m_focus_window;
    ResetFocusHandlerType m_reset_focus;
    ChangeWindowBorderColorHandlerType m_change_border_color;
    MoveAndResizeWindowHandlerType m_move_and_resize;
    CloseWindowHandlerType m_close_window;
  };
} // namespace ymwm::window
