#include "Manager.h"

#include <format>
#include <iostream>

namespace ymwm::window {

  Manager::Manager(UpdateWindowHandlerType&& update_window,
                   FocusWindowHandlerType&& focus_window,
                   ResetFocusHandlerType&& reset_focus,
                   ChangeWindowBorderColorHandlerType&& change_border_color,
                   MoveAndResizeWindowHandlerType&& move_and_resize)
      : m_update_window(update_window)
      , m_focus_window(focus_window)
      , m_reset_focus(reset_focus)
      , m_change_border_color(change_border_color)
      , m_move_and_resize(move_and_resize) {
    m_windows.reserve(5);
  }

  void Manager::add_window(const Window& w) noexcept {
    std::cout << std::format("{} {} {} {} {}\n", w.id, w.x, w.y, w.h, w.w);
    m_windows.push_back(w);
    m_windows.back().w = 200;
    m_windows.back().h = 200;
    m_update_window(m_windows.back());
    m_focus_window(m_windows.back());
  }

  void Manager::remove_window(environment::ID id) noexcept {
    auto erased_successfully = std::erase_if(
        m_windows, [id](const Window& w) -> bool { return id == w.id; });

    if (erased_successfully) {
      std::cout << std::format("Erased {} \n", id);
    }

    if (not m_windows.empty()) {
      m_focus_window(m_windows.back());
    } else {
      m_reset_focus();
    }
  }

  const std::vector<Window>& Manager::windows() const noexcept {
    return m_windows;
  }

  bool Manager::has_window(environment::ID id) const noexcept {
    return m_windows.end() !=
           std::find_if(m_windows.begin(),
                        m_windows.end(),
                        [id](const auto& w) -> bool { return id == w.id; });
  }

  void Manager::change_border_color(environment::ColorID color) noexcept {
    if (not m_windows.empty()) {
      m_windows.back().border_color = color;
      m_update_window(m_windows.back());
    }
  }
} // namespace ymwm::window
