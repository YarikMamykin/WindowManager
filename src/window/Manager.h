#pragma once
#include "Window.h"
#include "environment/ColorID.h"
#include "environment/ID.h"
#include "layouts/Layout.h"

#include <format>
#include <iostream>
#include <optional>
#include <vector>

namespace ymwm::window {
  template <typename Environment>
  struct Manager {
    using FocusedWindowType = std::optional<std::reference_wrapper<Window>>;

    Manager(Environment* env)
        : m_env(env)
        , m_focused_window_index(0ul) {
      m_windows.reserve(5);
    }

    inline void set_layout(const layouts::Layout& new_layout) noexcept {
      m_layout = new_layout;
      update_layout();
    }

    inline void update_layout() noexcept {
      update_layout_parameters();
      for (Window& w : m_windows) {
        m_layout.apply(w);
        m_env->move_and_resize(w);
      }
    }

    inline void update_layout_parameters() noexcept {
      auto [screen_width, screen_height] = m_env->screen_width_and_height();
      layouts::Layout::BasicParameters basic_parameters{
        .screen_width = screen_width,
        .screen_height = screen_height,
        .screen_margins = m_layout.basic_parameters.screen_margins,
        .focused_window_index = m_focused_window_index,
        .number_of_windows = m_windows.size()
      };
      m_layout.update(basic_parameters, m_layout.parameters);
    }

    inline void add_window(const Window& w) noexcept {
      std::cout << std::format("{} {} {} {} {}\n", w.id, w.x, w.y, w.h, w.w);
      m_windows.push_back(w);
      m_env->update_window_border(w);
      focus_next_window();
      update_layout();
    }

    inline void update_focus() noexcept {
      if (auto fw = focused_window()) {
        m_env->focus_window(fw->get());
        return;
      }

      m_env->reset_focus();
    }

    inline void remove_window(environment::ID id) noexcept {
      auto erased_successfully = std::erase_if(
          m_windows, [id](const Window& w) -> bool { return id == w.id; });

      if (erased_successfully) {
        std::cout << std::format("Erased {} \n", id);
      }

      focus_next_window();
      update_layout();
    }

    inline const std::vector<Window>& windows() const noexcept {
      return m_windows;
    }

    inline FocusedWindowType focused_window() noexcept {
      return m_windows.empty()
                 ? FocusedWindowType{ std::nullopt }
                 : FocusedWindowType{ m_windows.at(m_focused_window_index) };
    }

    inline bool has_window(environment::ID id) const noexcept {
      return m_windows.end() !=
             std::find_if(m_windows.begin(),
                          m_windows.end(),
                          [id](const auto& w) -> bool { return id == w.id; });
    }

    inline void change_border_color(environment::ColorID color) noexcept {
      if (auto fw = focused_window()) {
        fw->get().border_color = color;
        m_env->update_window(fw->get());
      }
    }

    inline void move_focused_window_to(int x, int y) noexcept {
      if (auto fw = focused_window()) {
        fw->get().x = x;
        fw->get().y = y;
        m_env->move_and_resize(fw->get());
      }
    }

    inline void focus_next_window() noexcept {
      if (m_windows.empty()) {
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
        return;
      }

      m_focused_window_index = m_focused_window_index == 0ul
                                   ? m_windows.size() - 1ul
                                   : m_focused_window_index - 1ul;
      update_focus();
    }

    inline void close_focused_window() noexcept {
      if (auto fw = focused_window()) {
        m_env->close_window(fw->get());
      }
    }

  private:
    std::vector<Window> m_windows;
    std::size_t m_focused_window_index;
    Environment* const m_env;
    layouts::Layout m_layout;
  };
} // namespace ymwm::window
