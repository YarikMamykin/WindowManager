#pragma once

#include "Window.h"
#include "common/Direction.h"
#include "environment/ID.h"
#include "layouts/Centered.h"
#include "layouts/Grid.h"
#include "layouts/Helpers.h"
#include "layouts/ParallelHorizontal.h"
#include "layouts/ParallelVertical.h"
#include "layouts/Parameters.h"
#include "layouts/StackHorizontalBottom.h"
#include "layouts/StackVerticalDouble.h"
#include "layouts/StackVerticalLeft.h"

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

      update_index(0ul);
    }

    inline void last_window() noexcept {
      if (m_windows.empty()) {
        return;
      }

      update_index(m_windows.size() - 1ul);
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

      if (auto w = window(); w.has_value() and wid == w->get().id) {
        return;
      }

      auto found_window =
          std::find_if(m_windows.cbegin(),
                       m_windows.cend(),
                       [wid](const auto& w) -> bool { return wid == w.id; });

      if (m_windows.cend() == found_window) {
        return;
      }

      update_index(std::distance(m_windows.cbegin(), found_window));
    }

    inline void window_by_index(std::size_t index) noexcept {
      if (m_windows.empty()) {
        return;
      }
      update_index(index);
    }

    inline void next_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }
      update_index(m_focused_window_index >= (m_windows.size() - 1ul)
                       ? 0ul
                       : m_focused_window_index + 1ul);
    }

    inline void prev_window() noexcept {
      if (m_windows.empty()) {
        m_env->reset_focus();
        return;
      }

      update_index(m_focused_window_index == 0ul
                       ? m_windows.size() - 1ul
                       : m_focused_window_index - 1ul);
    }

    inline bool is_last_window() const noexcept {
      return not m_windows.empty() and
             m_focused_window_index == m_windows.size() - 1ul;
    }

    inline bool is_first_window() const noexcept {
      return not m_windows.empty() and m_focused_window_index == 0ul;
    }

    inline void move(common::Direction direction,
                     const layouts::Parameters& parameters,
                     std::size_t number_of_windows) noexcept {

      std::visit(
          [direction, number_of_windows, this](const auto& params) -> void {
            this->move_on_layout(direction, params, number_of_windows);
          },
          parameters);
    }

    inline void reset() noexcept { m_env->reset_focus(); }

  private:
    inline void verify_index() noexcept {
      if (m_focused_window_index >= m_windows.size() and
          not m_windows.empty()) {
        m_focused_window_index = m_windows.size() - 1ul;
      }
    }

    inline void update_index(std::size_t new_index) noexcept {
      verify_index();

      m_before_focus_move();

      m_focused_window_index = new_index;
      verify_index();
      update();

      m_after_focus_move();
    }

    inline void move_focus_to_index(std::size_t index) noexcept {
      if (m_windows.empty()) {
        return;
      }

      update_index(index);
    }

    inline void move_on_layout(common::Direction direction,
                               const layouts::Grid& params,
                               std::size_t number_of_windows) noexcept {
      auto grid_size = params.grid_size;
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

    inline void
    move_on_layout(common::Direction direction,
                   const layouts::StackVerticalLeft& params,
                   [[maybe_unused]] std::size_t number_of_windows) noexcept {
      switch (direction) {
      case common::Direction::Left:
        if (is_first_window() and params.number_of_stack_windows > 0ul) {
          move_focus_to_index(1ul);
        }
        break;
      case common::Direction::Right:
        if (m_focused_window_index > 0ul) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Up:
        if (m_focused_window_index > 1ul) {
          move_focus_to_index(m_focused_window_index - 1ul);
        }
        break;
      case common::Direction::Down:
        if (not is_first_window() and not is_last_window()) {
          move_focus_to_index(m_focused_window_index + 1ul);
        }
        break;
      }
    }

    inline void
    move_on_layout(common::Direction direction,
                   const layouts::StackVerticalRight& params,
                   [[maybe_unused]] std::size_t number_of_windows) noexcept {
      switch (direction) {
      case common::Direction::Left:
        if (not is_first_window()) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Right:
        if (is_first_window() and params.number_of_stack_windows > 0ul) {
          move_focus_to_index(1ul);
        }
        break;
      case common::Direction::Up:
        if (m_focused_window_index > 1ul) {
          move_focus_to_index(m_focused_window_index - 1ul);
        }
        break;
      case common::Direction::Down:
        if (not is_first_window() and not is_last_window()) {
          move_focus_to_index(m_focused_window_index + 1ul);
        }
        break;
      }
    }

    inline void move_on_layout(common::Direction direction,
                               const layouts::StackVerticalDouble& params,
                               std::size_t) noexcept {

      if (m_windows.empty()) {
        return;
      }

      bool current_focused_is_main = is_first_window();
      bool current_focused_is_in_left_stack =
          0ul != (m_focused_window_index % 2ul);
      bool current_focused_is_in_right_stack =
          0ul == (m_focused_window_index % 2ul);
      bool left_stack_present = params.number_of_stack_windows > 0ul;
      bool right_stack_present = params.number_of_stack_windows > 1ul;
      bool current_focused_is_in_stack_and_not_first =
          (current_focused_is_in_left_stack and 1ul < m_focused_window_index) or
          (current_focused_is_in_right_stack and 2ul < m_focused_window_index);

      switch (direction) {
      case common::Direction::Left:
        if (current_focused_is_main and left_stack_present) {
          move_focus_to_index(1ul);
        } else if (current_focused_is_in_right_stack) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Right:
        if (current_focused_is_main and right_stack_present) {
          move_focus_to_index(2ul);
        } else if (current_focused_is_in_left_stack) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Up:
        if (current_focused_is_in_stack_and_not_first) {
          move_focus_to_index(m_focused_window_index - 2ul);
        }
        break;
      case common::Direction::Down:
        if ((current_focused_is_in_left_stack or
             current_focused_is_in_right_stack) and
            not is_last_window()) {
          move_focus_to_index(m_focused_window_index + 2ul);
        }
        break;
      }
    }

    inline void move_on_layout(common::Direction direction,
                               const layouts::StackHorizontalTop&,
                               std::size_t number_of_windows) noexcept {
      switch (direction) {
      case common::Direction::Left:
        if (not is_first_window() and m_focused_window_index > 1ul) {
          move_focus_to_index(m_focused_window_index - 1ul);
        }
        break;
      case common::Direction::Right:
        if (not is_last_window() and number_of_windows > 1ul) {
          move_focus_to_index(m_focused_window_index + 1ul);
        }
        break;
      case common::Direction::Up:
        if (not is_first_window()) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Down:
        if (is_first_window() and number_of_windows > 1ul) {
          move_focus_to_index(1ul);
        }
        break;
      }
    }

    inline void move_on_layout(common::Direction direction,
                               const layouts::StackHorizontalBottom&,
                               std::size_t number_of_windows) noexcept {
      switch (direction) {
      case common::Direction::Left:
        if (not is_first_window() and m_focused_window_index > 1ul) {
          move_focus_to_index(m_focused_window_index - 1ul);
        }
        break;
      case common::Direction::Right:
        if (not is_last_window() and number_of_windows > 1ul) {
          move_focus_to_index(m_focused_window_index + 1ul);
        }
        break;
      case common::Direction::Up:
        if (is_first_window() and number_of_windows > 1ul) {
          move_focus_to_index(1ul);
        }
        break;
      case common::Direction::Down:
        if (not is_first_window()) {
          move_focus_to_index(0ul);
        }
        break;
      }
    }

    inline void move_on_layout(common::Direction direction,
                               const layouts::StackHorizontalDouble& params,
                               std::size_t) noexcept {

      if (m_windows.empty()) {
        return;
      }

      bool current_focused_is_main = is_first_window();
      bool current_focused_is_in_top_stack =
          0ul != (m_focused_window_index % 2ul);
      bool current_focused_is_in_bottom_stack =
          0ul == (m_focused_window_index % 2ul);
      bool top_stack_present = params.number_of_stack_windows > 0ul;
      bool bottom_stack_present = params.number_of_stack_windows > 1ul;
      bool current_focused_is_in_stack_and_not_first =
          (current_focused_is_in_top_stack and 1ul < m_focused_window_index) or
          (current_focused_is_in_bottom_stack and 2ul < m_focused_window_index);

      switch (direction) {
      case common::Direction::Left:
        if (current_focused_is_in_stack_and_not_first) {
          move_focus_to_index(m_focused_window_index - 2ul);
        }
        break;
      case common::Direction::Right:
        if ((current_focused_is_in_bottom_stack or
             current_focused_is_in_top_stack) and
            not is_last_window()) {
          move_focus_to_index(m_focused_window_index + 2ul);
        }
        break;
      case common::Direction::Up:
        if (current_focused_is_main and top_stack_present) {
          move_focus_to_index(1ul);
        } else if (current_focused_is_in_bottom_stack) {
          move_focus_to_index(0ul);
        }
        break;
      case common::Direction::Down:
        if (current_focused_is_main and bottom_stack_present) {
          move_focus_to_index(2ul);
        } else if (current_focused_is_in_top_stack) {
          move_focus_to_index(0ul);
        }
        break;
      }
    }

    inline void
    move_on_layout(common::Direction, const auto&, std::size_t) noexcept {}

  private:
    const std::vector<Window>& m_windows;
    Environment* const m_env;
    std::size_t m_focused_window_index;
    std::function<void()> m_before_focus_move;
    std::function<void()> m_after_focus_move;
  };
} // namespace ymwm::window
