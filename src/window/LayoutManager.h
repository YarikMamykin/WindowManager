#pragma once
#include "Window.h"
#include "common/VariantInterfaceHelpers.h"
#include "config/Layout.h"
#include "layouts/Centered.h"
#include "layouts/Helpers.h"
#include "layouts/Layout.h"
#include "layouts/Parameters.h"
#include "layouts/StackHorizontalBottom.h"
#include "layouts/StackHorizontalTop.h"

#include <string_view>
#include <variant>
#include <vector>

namespace ymwm::window {
  template <typename Environment>
  struct LayoutManager {

    LayoutManager(std::vector<Window>& windows, Environment* const env)
        : m_windows(windows)
        , m_env(env) {}

    inline void update(const layouts::Parameters& parameters) {
      m_layout_parameters = parameters;
    }

    inline void update() noexcept {
      if (m_windows.empty()) {
        return;
      }

      auto layout = with_layout();
      for (Window& w : m_windows) {
        layout.apply(w);
        m_env->move_and_resize(w);
      }
    }

    inline void next() noexcept {
      static constexpr auto known_layouts =
          common::known_ids<layouts::Parameters>();
      auto current_layout_index = m_layout_parameters.index();
      auto next_index = current_layout_index < (known_layouts.size() - 1ul)
                            ? current_layout_index + 1ul
                            : 0ul;

      m_layout_parameters = *common::try_find_type<layouts::Parameters>(
          known_layouts.at(next_index));
      update();
    }

    inline void update_main_window_ratio(int diff) {
      layouts::update_main_window_ratio(m_layout_parameters, diff);
      update();
    }

    inline std::string_view current() const noexcept {
      return std::visit(
          [](const auto& p) -> std::string_view { return p.type; },
          m_layout_parameters);
    }

    inline layouts::Parameters parameters() noexcept {
      auto layout = with_layout();
      return layout.parameters;
    }

    template <typename T>
    inline std::optional<T> parameters() noexcept {
      auto p = parameters();
      return layouts::is<T>(p) ? std::optional<T>{ std::get<T>(p) }
                               : std::optional<T>{ std::nullopt };
    }

    inline void rotate() noexcept {
      m_layout_parameters = rotated_parameters();
      update();
    }

  private:
    inline layouts::Layout with_layout() noexcept {
      auto [screen_width, screen_height] = m_env->screen_width_and_height();

      m_basic_layout_parameters.screen_width = screen_width;
      m_basic_layout_parameters.screen_height = screen_height;
      m_basic_layout_parameters.number_of_windows = m_windows.size();
      m_basic_layout_parameters.screen_margins =
          config::layouts::screen_margins;

      auto layout =
          layouts::Layout{ .basic_parameters = m_basic_layout_parameters,
                           .parameters = m_layout_parameters };

      // Do any specific layout paramters update
      layout.update(m_basic_layout_parameters, m_layout_parameters);
      return layout;
    }

    inline layouts::Parameters rotated_parameters() const noexcept {
      if (layouts::is<layouts::StackVerticalRight>(m_layout_parameters)) {
        return layouts::StackHorizontalTop{};
      }
      if (layouts::is<layouts::StackVerticalLeft>(m_layout_parameters)) {
        return layouts::StackHorizontalBottom{};
      }
      if (layouts::is<layouts::StackVerticalDouble>(m_layout_parameters)) {
        return layouts::StackHorizontalDouble{};
      }
      if (layouts::is<layouts::StackHorizontalTop>(m_layout_parameters)) {
        return layouts::StackVerticalLeft{};
      }
      if (layouts::is<layouts::StackHorizontalBottom>(m_layout_parameters)) {
        return layouts::StackVerticalRight{};
      }
      if (layouts::is<layouts::StackHorizontalDouble>(m_layout_parameters)) {
        return layouts::StackVerticalDouble{};
      }

      return m_layout_parameters;
    }

  private:
    std::vector<Window>& m_windows;
    Environment* const m_env;
    layouts::Layout::BasicParameters m_basic_layout_parameters;
    layouts::Parameters m_layout_parameters;
  };
} // namespace ymwm::window
