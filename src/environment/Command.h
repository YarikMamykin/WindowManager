#pragma once

#include "CommandMacro.h"
#include "common/Color.h"
#include "common/Direction.h"
#include "environment/ID.h"

#include <optional>
#include <string>
#include <string_view>
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
  DEFINE_COMMAND(MoveFocusedWindowToNextGroup)
  DEFINE_COMMAND(MoveFocusedWindowBackward)
  DEFINE_COMMAND(MoveFocusedWindowToPreviousGroup)
  DEFINE_COMMAND(ChangeLayout)
  DEFINE_COMMAND_WITH_PARAMS_1(SetLayout, std::string_view layout);
  DEFINE_COMMAND_WITH_PARAMS_1(IncreaseMainWindowRatio, int diff{ 10 });
  DEFINE_COMMAND_WITH_PARAMS_1(DecreaseMainWindowRatio, int diff{ 10 });
  DEFINE_COMMAND(SwapFocusedWindowOnTop)
  DEFINE_COMMAND_WITH_PARAMS_1(MoveFocus, common::Direction direction);
  DEFINE_COMMAND(RotateStackLayout)
  DEFINE_COMMAND(NextLanguageLayout);
  DEFINE_COMMAND(AddWindow);
  DEFINE_COMMAND(UpdateWindowName);
  DEFINE_COMMAND_WITH_PARAMS_1(FocusWindow, environment::ID wid);
  DEFINE_COMMAND_WITH_PARAMS_1(RemoveWindow, environment::ID wid);
  DEFINE_COMMAND(AddGroup);
  DEFINE_COMMAND(MoveToNextGroup);
  DEFINE_COMMAND(MoveToPrevGroup);
  DEFINE_COMMAND(RemoveGroup);
  DEFINE_COMMAND(TakeScreenshot);
  DEFINE_COMMAND(SaveScreenshotCoords);

  using Command = std::variant<ExitRequested,
                               RunTerminal,
                               ChangeBorderColor,
                               MoveWindowRight,
                               CloseWindow,
                               FocusNextWindow,
                               FocusPrevWindow,
                               MoveFocusedWindowForward,
                               MoveFocusedWindowToNextGroup,
                               MoveFocusedWindowBackward,
                               MoveFocusedWindowToPreviousGroup,
                               ChangeLayout,
                               SetLayout,
                               IncreaseMainWindowRatio,
                               DecreaseMainWindowRatio,
                               SwapFocusedWindowOnTop,
                               MoveFocus,
                               RotateStackLayout,
                               NextLanguageLayout,
                               AddWindow,
                               UpdateWindowName,
                               FocusWindow,
                               RemoveWindow,
                               AddGroup,
                               MoveToNextGroup,
                               MoveToPrevGroup,
                               RemoveGroup,
                               TakeScreenshot,
                               SaveScreenshotCoords>;

} // namespace ymwm::environment::commands
