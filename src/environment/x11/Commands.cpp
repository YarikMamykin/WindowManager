#include "common/VariantInterfaceHelpers.h"
#include "environment/Command.h"
#include "environment/Environment.h"
#include "events/Event.h"
#include "events/MouseOverWindow.h"
#include "events/WindowRemoved.h"
#include "layouts/Grid.h"
#include "layouts/Parameters.h"

#include <cstdlib>
#include <string>
#include <thread>
#include <variant>

namespace ymwm::environment::commands {

  void ExitRequested::execute(Environment& e, const events::Event&) const {
    e.request_exit();
  }
  void RunTerminal::execute(Environment& e, const events::Event&) const {
    std::thread([&terminal = path]() -> void {
      std::system(terminal.c_str());
    }).detach();
  }

  void ChangeBorderColor::execute(Environment& e, const events::Event&) const {
    if (auto fw = e.manager().focus().window()) {
      e.manager().update_focused_window_border(fw->get().border_width, color);
    }
  }

  void MoveWindowRight::execute(Environment& e, const events::Event&) const {
    e.manager().move_focused_window_to(100, 0);
  }

  void CloseWindow::execute(Environment& e, const events::Event&) const {
    e.manager().close_focused_window();
  }

  void FocusNextWindow::execute(Environment& e, const events::Event&) const {
    e.manager().focus().next_window();
  }

  void FocusPrevWindow::execute(Environment& e, const events::Event&) const {
    e.manager().focus().prev_window();
  }

  void MoveFocusedWindowForward::execute(Environment& e,
                                         const events::Event&) const {
    e.manager().move_focused_window_forward();
  }

  void MoveFocusedWindowBackward::execute(Environment& e,
                                          const events::Event&) const {
    e.manager().move_focused_window_backward();
  }

  void ChangeLayout::execute(Environment& e, const events::Event&) const {
    e.manager().layout().next();
  }

  void SetLayout::execute(Environment& e, const events::Event&) const {
    using namespace ymwm::common;
    using namespace ymwm::layouts;
    e.manager().layout().update(*try_find_type<Parameters>(layout));
    e.manager().layout().update();
  }

  void IncreaseMainWindowRatio::execute(Environment& e,
                                        const events::Event&) const {
    e.manager().layout().update_main_window_ratio(diff < 0 ? 0 - diff : diff);
  }

  void DecreaseMainWindowRatio::execute(Environment& e,
                                        const events::Event&) const {
    e.manager().layout().update_main_window_ratio(diff > 0 ? 0 - diff : diff);
  }

  void SwapFocusedWindowOnTop::execute(Environment& e,
                                       const events::Event&) const {
    e.manager().swap_focused_window_with_top();
  }

  void MoveFocusOnGrid::execute(Environment& e, const events::Event&) const {
    if (auto parameters = e.manager().layout().parameters<layouts::Grid>()) {
      e.manager().focus().move_on_grid(
          direction, parameters->grid_size, e.manager().windows().size());
    }
  }

  void RotateStackLayout::execute(Environment& e, const events::Event&) const {
    e.manager().layout().rotate();
  }

  void NextLanguageLayout::execute(Environment& e, const events::Event&) const {
    e.next_keyboard_layout();
  }

  void AddWindow::execute(Environment& e, const events::Event& event) const {
    if (const auto* w = std::get_if<events::WindowAdded>(&event)) {
      e.manager().add_window(w->w);
    }
  }

  void UpdateWindowName::execute(Environment& e,
                                 const events::Event& event) const {
    if (const auto* ev = std::get_if<events::WindowNameUpdated>(&event)) {
      e.manager().update_window_name(ev->wid, std::u8string(ev->wname));
    }
  }

  void FocusWindow::execute(Environment& e, const events::Event& event) const {
    if (const auto* ev = std::get_if<events::MouseOverWindow>(&event);
        ev and
        not layouts::is<layouts::Centered>(e.manager().layout().parameters())) {
      e.manager().focus().window(ev->wid);
    }
  }

  void RemoveWindow::execute(Environment& e, const events::Event& event) const {
    if (const auto* ev = std::get_if<events::WindowRemoved>(&event)) {
      e.manager().remove_window(ev->wid);
      if (e.manager().windows().empty()) {
        e.group().remove(e.group().active());
      }
    }
  }

  void AddGroup::execute(Environment& e, const events::Event& event) const {
    e.group().add();
  }

  void MoveToNextGroup::execute(Environment& e,
                                const events::Event& event) const {
    e.group().next();
  }

  void MoveToPrevGroup::execute(Environment& e,
                                const events::Event& event) const {
    e.group().prev();
  }

  void RemoveGroup::execute(Environment& e, const events::Event& event) const {
    e.group().remove(e.group().active());
  }
} // namespace ymwm::environment::commands
