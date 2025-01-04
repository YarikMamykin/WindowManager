#include "environment/Command.h"
#include "environment/Environment.h"

#include <cstdlib>
#include <thread>

namespace ymwm::environment::commands {

  void Dummy::execute([[maybe_unused]] Environment&) const {}
  void ExitRequested::execute(Environment& e) const { e.request_exit(); }
  void RunTerminal::execute(Environment& e) const {
    std::thread([]() -> void {
      std::system("/usr/bin/alacritty -o \"window.dimensions={columns=20, "
                  "lines=20}\"");
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
    e.manager().focus_next_window();
  }

  void FocusPrevWindow::execute(Environment& e) const {
    e.manager().focus_prev_window();
  }

  void MoveFocusedWindowForward::execute(Environment& e) const {
    e.manager().move_focused_window_forward();
  }

  void MoveFocusedWindowBackward::execute(Environment& e) const {
    e.manager().move_focused_window_backward();
  }
} // namespace ymwm::environment::commands
