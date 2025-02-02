#include "Map.h"

#include "AbstractKeyCode.h"
#include "AbstractKeyMask.h"
#include "common/Direction.h"
#include "environment/Command.h"
#include "events/AbstractKeyPress.h"
#include "layouts/Grid.h"
#include "layouts/Maximized.h"
#include "layouts/StackVerticalDouble.h"

namespace ymwm::events {
  Map default_event_map() noexcept {
    Map bindings;

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::E,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::ExitRequested{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::T,
            .mask = ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::RunTerminal{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::C,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::CloseWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::J,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusNextWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::K,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::FocusPrevWindow{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::J,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowForward{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::K,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::MoveFocusedWindowBackward{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::L,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::ChangeLayout{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::L,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::IncreaseMainWindowRatio{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::H,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::DecreaseMainWindowRatio{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::Enter,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::SwapFocusedWindowOnTop{});

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::G,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::SetLayout{ ymwm::layouts::Grid::type });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::M,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::SetLayout{
            ymwm::layouts::Maximised::type });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::S,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::SetLayout{
            ymwm::layouts::StackVerticalDouble::type });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::R,
            .mask = ymwm::events::AbstractKeyMask::Alt },
        ymwm::environment::commands::RunTerminal{ "/usr/bin/dmenu_run" });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::J,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::MoveFocusOnGrid{
            common::Direction::Down });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::K,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::MoveFocusOnGrid{ common::Direction::Up });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::H,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::MoveFocusOnGrid{
            common::Direction::Left });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::L,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Ctrl },
        ymwm::environment::commands::MoveFocusOnGrid{
            common::Direction::Right });

    bindings.emplace(
        ymwm::events::AbstractKeyPress{
            .code = ymwm::events::AbstractKeyCode::R,
            .mask = ymwm::events::AbstractKeyMask::Alt |
                    ymwm::events::AbstractKeyMask::Shift },
        ymwm::environment::commands::RotateStackLayout{});

    return bindings;
  }

} // namespace ymwm::events
