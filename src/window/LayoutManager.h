#pragma once
#include "Window.h"
#include "layouts/Layout.h"

#include <vector>

namespace ymwm::window {
  template <typename Environment>
  struct LayoutManager {

    LayoutManager(std::vector<Window>& windows, Environment* const env)
        : m_windows(windows)
        , m_env(env) {}

    inline void update(const layouts::Margin& screen_margins) {
      m_basic_layout_parameters.screen_margins = screen_margins;
    }

    inline void update(const layouts::Parameters& parameters) {
      m_layout_parameters = parameters;
    }

    inline void update() noexcept {
      auto layout = with_layout();
      for (Window& w : m_windows) {
        layout.apply(w);
        m_env->move_and_resize(w);
      }
    }

  private:
    inline layouts::Layout with_layout() noexcept {
      auto [screen_width, screen_height] = m_env->screen_width_and_height();

      m_basic_layout_parameters.screen_width = screen_width;
      m_basic_layout_parameters.screen_height = screen_height;
      m_basic_layout_parameters.focused_window_index = 0ul;
      m_basic_layout_parameters.number_of_windows = m_windows.size();

      auto layout =
          layouts::Layout{ .basic_parameters = m_basic_layout_parameters,
                           .parameters = m_layout_parameters };

      // Do any specific layout paramters update
      layout.update(m_basic_layout_parameters, m_layout_parameters);
      return layout;
    }

  private:
    std::vector<Window>& m_windows;
    Environment* const m_env;
    layouts::Layout::BasicParameters m_basic_layout_parameters;
    layouts::Parameters m_layout_parameters;
  };
} // namespace ymwm::window
