#include "environment/Command.h"
#include "environment/Environment.h"

#include <cstdlib>
#include <thread>

namespace ymwm::environment::commands {

  void ExitRequested::execute(Environment& e) const { e.request_exit(); }
  void RunTerminal::execute(Environment& e) const {
    std::thread([&terminal = path]() -> void {
      std::system(terminal.c_str());
    }).detach();
  }

  void ChangeBorderColor::execute(Environment& e) const {
    if (auto fw = e.manager().focus().window()) {
      e.manager().update_focused_window_border(fw->get().border_width, color);
    }
  }

  void MoveWindowRight::execute(Environment& e) const {
    e.manager().move_focused_window_to(100, 0);
  }

  void CloseWindow::execute(Environment& e) const {
    e.manager().close_focused_window();
  }

  void FocusNextWindow::execute(Environment& e) const {
    e.manager().focus().next_window();
  }

  void FocusPrevWindow::execute(Environment& e) const {
    e.manager().focus().prev_window();
  }

  void MoveFocusedWindowForward::execute(Environment& e) const {
    e.manager().move_focused_window_forward();
  }

  void MoveFocusedWindowBackward::execute(Environment& e) const {
    e.manager().move_focused_window_backward();
  }

  void ChangeLayout::execute(Environment& e) const {
    e.manager().layout().next();
  }

  void IncreaseMainWindowRatio::execute(Environment& e) const {
    e.manager().layout().update_main_window_ratio(diff < 0 ? 0 - diff : diff);
  }

  void DecreaseMainWindowRatio::execute(Environment& e) const {
    e.manager().layout().update_main_window_ratio(diff > 0 ? 0 - diff : diff);
  }
} // namespace ymwm::environment::commands
