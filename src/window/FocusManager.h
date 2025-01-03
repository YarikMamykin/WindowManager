#pragma once

#include "Window.h"

#include <optional>
#include <vector>

namespace ymwm::window {
  template <typename Environment>
  struct FocusManager {
    using FocusedWindow = std::optional<std::reference_wrapper<Window>>;

    FocusManager(const std::vector<Window>& windows, Environment* const env)
        : m_windows(windows)
        , m_env(env)
        , m_focused_window_index(0ul) {}

    inline std::size_t window_index() const noexcept {
      return m_focused_window_index;
    }

    inline void update() const noexcept {
      if (auto fw = window()) {
        m_env->focus_window(fw->get());
        return;
      }

      m_env->reset_focus();
    }

    inline void update_index() noexcept {
      if (m_focused_window_index >= m_windows.size() and
          not m_windows.empty()) {
        m_focused_window_index = m_windows.size() - 1ul;
      }
    }

    inline void last_window() noexcept {
      if (m_windows.empty()) {
        return;
      }
      m_focused_window_index = m_windows.size() - 1ul;
      update();
    }

    inline FocusedWindow window() const noexcept {
      return m_windows.empty() ? FocusedWindow{ std::nullopt }
                               : FocusedWindow{ const_cast<Window&>(
                                     m_windows.at(m_focused_window_index)) };
    }

    inline void next_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      m_focused_window_index =
          m_focused_window_index >= (m_windows.size() - 1ul)
              ? 0ul
              : m_focused_window_index + 1ul;
      update();
    }

    inline void prev_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      m_focused_window_index = m_focused_window_index == 0ul
                                   ? m_windows.size() - 1ul
                                   : m_focused_window_index - 1ul;
      update();
    }

    inline bool is_last_window() const noexcept {
      return not m_windows.empty() and
             m_focused_window_index == m_windows.size() - 1ul;
    }

    inline bool is_first_window() const noexcept {
      return not m_windows.empty() and m_focused_window_index == 0ul;
    }

  private:
    const std::vector<Window>& m_windows;
    Environment* const m_env;
    std::size_t m_focused_window_index;
  };
} // namespace ymwm::window
