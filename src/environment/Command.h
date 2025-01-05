#pragma once

#include "common/Color.h"

#include <string_view>
#include <variant>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment::commands {
  struct ExitRequested {
    static inline constexpr std::string_view type{ "ExitRequested" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct RunTerminal {
    static inline constexpr std::string_view type{ "RunTerminal" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct ChangeBorderColor {
    static inline constexpr std::string_view type{ "ChangeBorderColor" };
    void execute([[maybe_unused]] Environment&) const;
    common::Color color;
  };

  struct MoveWindowRight {
    static inline constexpr std::string_view type{ "MoveWindowRight" };
    void execute([[maybe_unused]] Environment&) const;
    int dx;
  };

  struct CloseWindow {
    static inline constexpr std::string_view type{ "CloseWindow" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct FocusNextWindow {
    static inline constexpr std::string_view type{ "FocusNextWindow" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct FocusPrevWindow {
    static inline constexpr std::string_view type{ "FocusPrevWindow" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct MoveFocusedWindowForward {
    static inline constexpr std::string_view type{ "MoveFocusedWindowForward" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct MoveFocusedWindowBackward {
    static inline constexpr std::string_view type{
      "MoveFocusedWindowBackward"
    };
    void execute([[maybe_unused]] Environment&) const;
  };

  using Command = std::variant<ExitRequested,
                               RunTerminal,
                               ChangeBorderColor,
                               MoveWindowRight,
                               CloseWindow,
                               FocusNextWindow,
                               FocusPrevWindow,
                               MoveFocusedWindowForward,
                               MoveFocusedWindowBackward>;
} // namespace ymwm::environment::commands
