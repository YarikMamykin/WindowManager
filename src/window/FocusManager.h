#pragma once

#include "Window.h"

#include <functional>
#include <optional>
#include <vector>

namespace ymwm::window {
  template <typename Environment>
  struct FocusManager {
    using FocusedWindow = std::optional<std::reference_wrapper<Window>>;

    FocusManager(const std::vector<Window>& windows,
                 Environment* const env,
                 std::function<void()>&& before_focus_move,
                 std::function<void()>&& after_focus_move)
        : m_windows(windows)
        , m_env(env)
        , m_focused_window_index(0ul)
        , m_before_focus_move(before_focus_move)
        , m_after_focus_move(after_focus_move) {}

    inline std::size_t window_index() const noexcept {
      return m_focused_window_index;
    }

    inline void update() noexcept {
      if (auto fw = window()) {
        m_env->focus_window(fw->get());
        return;
      }

      m_env->reset_focus();
    }

    inline void first_window() noexcept {
      if (m_windows.empty()) {
        return;
      }

      update_index();

      m_before_focus_move();

      m_focused_window_index = 0ul;
      update();

      m_after_focus_move();
    }

    inline void last_window() noexcept {
      if (m_windows.empty()) {
        return;
      }

      update_index();

      m_before_focus_move();

      m_focused_window_index = m_windows.size() - 1ul;
      update();

      m_after_focus_move();
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
      update_index();

      m_before_focus_move();

      m_focused_window_index =
          m_focused_window_index >= (m_windows.size() - 1ul)
              ? 0ul
              : m_focused_window_index + 1ul;
      update();

      m_after_focus_move();
    }

    inline void prev_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      update_index();

      m_before_focus_move();

      m_focused_window_index = m_focused_window_index == 0ul
                                   ? m_windows.size() - 1ul
                                   : m_focused_window_index - 1ul;
      update();

      m_after_focus_move();
    }

    inline bool is_last_window() const noexcept {
      return not m_windows.empty() and
             m_focused_window_index == m_windows.size() - 1ul;
    }

    inline bool is_first_window() const noexcept {
      return not m_windows.empty() and m_focused_window_index == 0ul;
    }

  private:
    inline void update_index() noexcept {
      if (m_focused_window_index >= m_windows.size() and
          not m_windows.empty()) {
        m_focused_window_index = m_windows.size() - 1ul;
      }
    }

  private:
    const std::vector<Window>& m_windows;
    Environment* const m_env;
    std::size_t m_focused_window_index;
    std::function<void()> m_before_focus_move;
    std::function<void()> m_after_focus_move;
  };
} // namespace ymwm::window
