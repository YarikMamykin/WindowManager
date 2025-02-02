#pragma once

#include "Window.h"
#include "common/Direction.h"
#include "environment/ID.h"

#include <algorithm>
#include <functional>
#include <iterator>
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

    inline void window(environment::ID wid) noexcept {
      if (m_windows.empty()) {
        return;
      }

      auto found_window =
          std::find_if(m_windows.cbegin(),
                       m_windows.cend(),
                       [wid](const auto& w) -> bool { return wid == w.id; });

      if (m_windows.cend() == found_window) {
        return;
      }

      update_index();

      m_before_focus_move();

      m_focused_window_index = std::distance(m_windows.cbegin(), found_window);
      update();

      m_after_focus_move();
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

    inline void move_on_grid(common::Direction direction,
                             std::size_t grid_size,
                             std::size_t number_of_windows) noexcept {
      switch (direction) {
      case common::Direction::Left:
        if (0 != (m_focused_window_index % grid_size)) {
          move_focus_to_index(m_focused_window_index - 1);
        }
        break;
      case common::Direction::Right:
        if ((grid_size - 1ul) != (m_focused_window_index % grid_size) and
            number_of_windows > (m_focused_window_index + 1)) {
          move_focus_to_index(m_focused_window_index + 1);
        }
        break;
      case common::Direction::Up:
        if (m_focused_window_index >= grid_size) {
          move_focus_to_index(m_focused_window_index - grid_size);
        }
        break;
      case common::Direction::Down:
        if ((m_focused_window_index + grid_size) < number_of_windows) {
          move_focus_to_index(m_focused_window_index + grid_size);
        }
        break;
      }
    }

  private:
    inline void update_index() noexcept {
      if (m_focused_window_index >= m_windows.size() and
          not m_windows.empty()) {
        m_focused_window_index = m_windows.size() - 1ul;
      }
    }

    inline void move_focus_to_index(std::size_t index) noexcept {
      if (m_windows.empty()) {
        return;
      }

      update_index();

      m_before_focus_move();

      m_focused_window_index = index;
      update();

      m_after_focus_move();
    }

  private:
    const std::vector<Window>& m_windows;
    Environment* const m_env;
    std::size_t m_focused_window_index;
    std::function<void()> m_before_focus_move;
    std::function<void()> m_after_focus_move;
  };
} // namespace ymwm::window
