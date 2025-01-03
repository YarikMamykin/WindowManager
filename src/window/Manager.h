#pragma once
#include "FocusManager.h"
#include "LayoutManager.h"
#include "Window.h"
#include "common/Color.h"
#include "environment/ID.h"

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
        , m_focus_manager(m_windows, env)
        , m_layout_manager(m_windows, env) {
      m_windows.reserve(5);
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
      focus_next_window();
      layout().update();
    }

    inline void remove_window(environment::ID id) noexcept {
      auto erased_successfully = std::erase_if(
          m_windows, [id](const Window& w) -> bool { return id == w.id; });

      if (erased_successfully) {
        std::cout << std::format("Erased {} \n", id);
        focus().update_index();
        focus_prev_window();
        layout().update();
      }
    }

    inline const std::vector<Window>& windows() const noexcept {
      return m_windows;
    }

    inline void change_border_color(const common::Color& color) noexcept {
      if (auto fw = focus().window()) {
        fw->get().border_color = color;
        m_env->update_window(fw->get());
      }
    }

    inline void move_focused_window_to(int x, int y) noexcept {
      if (auto fw = focus().window()) {
        fw->get().x = x;
        fw->get().y = y;
        m_env->move_and_resize(fw->get());
      }
    }

    inline void move_focused_window_forward() noexcept {
      if (auto fw = focus().window()) {
        if (focus().is_last_window()) {
          std::swap(m_windows.back(), m_windows.front());
        } else {
          std::swap(m_windows.at(focus().window_index()),
                    m_windows.at(focus().window_index() + 1ul));
        }
        layout().update();
        focus_next_window();
      }
    }

    inline void move_focused_window_backward() noexcept {
      if (auto fw = focus().window()) {
        if (focus().is_first_window()) {
          std::swap(m_windows.front(), m_windows.back());
        } else {
          std::swap(m_windows.at(focus().window_index()),
                    m_windows.at(focus().window_index() - 1ul));
        }
        layout().update();
        focus_prev_window();
      }
    }

    inline void close_focused_window() noexcept {
      if (auto fw = focus().window()) {
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

    inline void focus_next_window() noexcept {
      if (auto fw = focus().window()) {
        const common::Color regular_window_border_color{ 0xff, 0x0, 0x0 };
        change_border_color(regular_window_border_color);

        focus().next_window();

        const common::Color focused_window_border_color{ 0x0, 0xff, 0x0 };
        change_border_color(focused_window_border_color);
      }
    }

    inline void focus_prev_window() noexcept {
      if (auto fw = focus().window()) {
        const common::Color regular_window_border_color{ 0xff, 0x0, 0x0 };
        change_border_color(regular_window_border_color);

        focus().prev_window();

        const common::Color focused_window_border_color{ 0x0, 0xff, 0x0 };
        change_border_color(focused_window_border_color);
      }
    }

    inline FocusManager<Environment>& focus() noexcept {
      return m_focus_manager;
    }

    inline LayoutManager<Environment>& layout() noexcept {
      return m_layout_manager;
    }

  private:
    std::vector<Window> m_windows;
    Environment* const m_env;
    layouts::Layout m_layout;
    FocusManager<Environment> m_focus_manager;
    LayoutManager<Environment> m_layout_manager;
  };
} // namespace ymwm::window
