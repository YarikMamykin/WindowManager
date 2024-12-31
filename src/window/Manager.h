#pragma once
#include "FocusManager.h"
#include "Window.h"
#include "common/Color.h"
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
        , m_focus_manager(m_windows, env) {
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
        .focused_window_index = 0ul,
        .number_of_windows = m_windows.size()
      };
      m_layout.update(basic_parameters, m_layout.parameters);
    }

    inline void add_window(const Window& w) noexcept {
      std::cout << std::format("{} {} {} {} {} {}\n",
                               w.id,
                               w.x,
                               w.y,
                               w.h,
                               w.w,
                               reinterpret_cast<const char*>(w.name.data()));
      m_windows.push_back(w);
      m_env->update_window_border(w);
      m_focus_manager.focus_last_window();
      update_layout();
    }

    inline void remove_window(environment::ID id) noexcept {
      auto erased_successfully = std::erase_if(
          m_windows, [id](const Window& w) -> bool { return id == w.id; });

      if (erased_successfully) {
        std::cout << std::format("Erased {} \n", id);
      }

      m_focus_manager.focus_next_window();
      update_layout();
    }

    inline const std::vector<Window>& windows() const noexcept {
      return m_windows;
    }

    inline bool has_window(environment::ID id) const noexcept {
      return m_windows.end() !=
             std::find_if(m_windows.begin(),
                          m_windows.end(),
                          [id](const auto& w) -> bool { return id == w.id; });
    }

    inline void change_border_color(const common::Color& color) noexcept {
      if (auto fw = m_focus_manager.focused_window()) {
        fw->get().border_color = color;
        m_env->update_window(fw->get());
      }
    }

    inline void move_focused_window_to(int x, int y) noexcept {
      if (auto fw = m_focus_manager.focused_window()) {
        fw->get().x = x;
        fw->get().y = y;
        m_env->move_and_resize(fw->get());
      }
    }

    inline void move_focused_window_forward() noexcept {
      if (auto fw = m_focus_manager.focused_window()) {
        if (m_focus_manager.last_window_focused()) {
          std::swap(m_windows.back(), m_windows.front());
        } else {
          std::swap(m_windows.at(m_focus_manager.focused_window_index()),
                    m_windows.at(m_focus_manager.focused_window_index() + 1ul));
        }
        update_layout();
        m_focus_manager.focus_next_window();
      }
    }

    inline void move_focused_window_backward() noexcept {
      if (auto fw = m_focus_manager.focused_window()) {
        if (m_focus_manager.first_window_focused()) {
          std::swap(m_windows.front(), m_windows.back());
        } else {
          std::swap(m_windows.at(m_focus_manager.focused_window_index()),
                    m_windows.at(m_focus_manager.focused_window_index() - 1ul));
        }
        update_layout();
        m_focus_manager.focus_prev_window();
      }
    }

    inline void close_focused_window() noexcept {
      if (auto fw = m_focus_manager.focused_window()) {
        m_env->close_window(fw->get());
      }
    }

    inline void update_window_name(environment::ID wid,
                                   std::u8string&& new_name) noexcept {
      auto wit =
          std::find_if(m_windows.begin(),
                       m_windows.end(),
                       [wid](const auto& w) -> bool { return wid == w.id; });
      if (m_windows.end() != wit) {
        wit->name = std::move(new_name);
      }
    }

    inline FocusedWindowType focused_window() noexcept {
      return m_focus_manager.focused_window();
    }

    inline void focus_next_window() noexcept {
      m_focus_manager.focus_next_window();
    }

    inline void focus_prev_window() noexcept {
      m_focus_manager.focus_prev_window();
    }

  private:
    std::vector<Window> m_windows;
    Environment* const m_env;
    layouts::Layout m_layout;
    FocusManager<Environment> m_focus_manager;
  };
} // namespace ymwm::window
