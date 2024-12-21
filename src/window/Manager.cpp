#include "Manager.h"

#include "environment/Environment.h"

#include <format>
#include <iostream>

namespace ymwm::window {

  Manager::Manager(environment::Environment* env)
      : m_env(env) {
    m_windows.reserve(5);
  }

  void Manager::add_window(const Window& w) noexcept {
    std::cout << std::format("{} {} {} {} {}\n", w.id, w.x, w.y, w.h, w.w);
    m_windows.push_back(w);
    m_windows.back().w = 200;
    m_windows.back().h = 200;
    m_env->update_window(m_windows.back());
    m_env->focus_window(m_windows.back());
  }

  void Manager::move_focused_window_to(int x, int y) noexcept {
    if (m_windows.empty()) {
      return;
    }
    m_windows.back().x = x;
    m_windows.back().y = y;
    m_env->move_and_resize(m_windows.back());
  }

  void Manager::remove_window(environment::ID id) noexcept {
    auto erased_successfully = std::erase_if(
        m_windows, [id](const Window& w) -> bool { return id == w.id; });

    if (erased_successfully) {
      std::cout << std::format("Erased {} \n", id);
    }

    if (not m_windows.empty()) {
      m_env->focus_window(m_windows.back());
    } else {
      m_env->reset_focus();
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
      m_env->update_window(m_windows.back());
    }
  }

  void Manager::close_window(environment::ID id) noexcept {
    if (1ul < m_windows.size()) {
      m_env->focus_window(*std::prev(m_windows.end(), 2));
      m_env->close_window(id);
      return;
    }

    m_env->reset_focus();
    m_env->close_window(id);
  }
} // namespace ymwm::window
