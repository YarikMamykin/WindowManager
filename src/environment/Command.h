#pragma once

#include "CommandMacro.h"
#include "common/Color.h"

#include <optional>
#include <string>
#include <variant>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment::commands {
  DEFINE_COMMAND(ExitRequested)
  DEFINE_COMMAND_WITH_PARAMS_1(RunTerminal,
                               std::string path{ "/usr/bin/alacritty" })
  DEFINE_COMMAND_WITH_PARAMS_1(ChangeBorderColor, common::Color color);
  DEFINE_COMMAND_WITH_PARAMS_1(MoveWindowRight, int dx{ 10 });
  DEFINE_COMMAND(CloseWindow)
  DEFINE_COMMAND(FocusNextWindow)
  DEFINE_COMMAND(FocusPrevWindow)
  DEFINE_COMMAND(MoveFocusedWindowForward)
  DEFINE_COMMAND(MoveFocusedWindowBackward)
  DEFINE_COMMAND(ChangeLayout)

  using Command = std::variant<ExitRequested,
                               RunTerminal,
                               ChangeBorderColor,
                               MoveWindowRight,
                               CloseWindow,
                               FocusNextWindow,
                               FocusPrevWindow,
                               MoveFocusedWindowForward,
                               MoveFocusedWindowBackward,
                               ChangeLayout>;

  template <std::size_t Index =
                std::variant_size_v<environment::commands::Command> - 1ul>
  static inline std::optional<environment::commands::Command>
  try_find_command(std::string_view command_type) noexcept {
    if constexpr (0ul <= Index) {
      using CommandType =
          std::variant_alternative_t<Index, environment::commands::Command>;
      if (command_type == CommandType::type) {
        return CommandType{};
      }

      if constexpr (0ul == Index) {
        return std::nullopt;
      } else {
        return try_find_command<Index - 1ul>(command_type);
      }

    } else {
      return std::nullopt;
    }
  }
} // namespace ymwm::environment::commands
