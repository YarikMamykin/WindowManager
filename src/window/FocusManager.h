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

    inline std::size_t focused_window_index() const noexcept {
      return m_focused_window_index;
    }

    inline void update_focus() const noexcept {
      if (auto fw = focused_window()) {
        m_env->focus_window(fw->get());
        return;
      }

      m_env->reset_focus();
    }

    inline void focus_last_window() noexcept {
      if (m_windows.empty()) {
        return;
      }
      m_focused_window_index = m_windows.size() - 1ul;
      update_focus();
    }

    inline FocusedWindow focused_window() const noexcept {
      return m_windows.empty() ? FocusedWindow{ std::nullopt }
                               : FocusedWindow{ const_cast<Window&>(
                                     m_windows.at(m_focused_window_index)) };
    }

    inline void focus_next_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      m_focused_window_index =
          m_focused_window_index >= (m_windows.size() - 1ul)
              ? 0ul
              : m_focused_window_index + 1ul;
      update_focus();
    }

    inline void focus_prev_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      m_focused_window_index = m_focused_window_index == 0ul
                                   ? m_windows.size() - 1ul
                                   : m_focused_window_index - 1ul;
      update_focus();
    }

    inline bool last_window_focused() const noexcept {
      return not m_windows.empty() and
             m_focused_window_index == m_windows.size() - 1ul;
    }

    inline bool first_window_focused() const noexcept {
      return not m_windows.empty() and m_focused_window_index == 0ul;
    }

  private:
    const std::vector<Window>& m_windows;
    Environment* const m_env;
    std::size_t m_focused_window_index;
  };
} // namespace ymwm::window
