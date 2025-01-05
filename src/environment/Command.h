#pragma once

#include "CommandMacro.h"
#include "common/Color.h"

#include <variant>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment::commands {
  DEFINE_COMMAND(ExitRequested)
  DEFINE_COMMAND(RunTerminal)
  DEFINE_COMMAND_WITH_PARAMS_1(ChangeBorderColor, common::Color color);
  DEFINE_COMMAND_WITH_PARAMS_1(MoveWindowRight, int dx);
  DEFINE_COMMAND(CloseWindow)
  DEFINE_COMMAND(FocusNextWindow)
  DEFINE_COMMAND(FocusPrevWindow)
  DEFINE_COMMAND(MoveFocusedWindowForward)
  DEFINE_COMMAND(MoveFocusedWindowBackward)

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
