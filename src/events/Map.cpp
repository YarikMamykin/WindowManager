#include "Map.h"

#include "AbstractKeyCode.h"
#include "AbstractKeyMask.h"
#include "environment/Command.h"
#include "events/AbstractKeyPress.h"

namespace ymwm::events {
  Map default_event_map() noexcept {
    Map bindings;

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::e,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::ExitRequested{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::t,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::RunTerminal{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::c,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::CloseWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::j,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusNextWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::k,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusPrevWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::j,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowForward{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::k,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowBackward{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::l,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::ChangeLayout{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::l,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::IncreaseMainWindowRatio{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::h,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::DecreaseMainWindowRatio{});

    return bindings;
  }

} // namespace ymwm::events
