#pragma once

#include "checks/Check.h"
#include "environment/Command.h"

#include <set>
#include <string_view>
#include <unordered_map>

namespace ymwm::checks {
  using Checks = std::set<std::string_view>;
  using Map = std::unordered_map<std::string_view, Checks>;

  static const inline Map checks_map{
    {               ymwm::environment::commands::CloseWindow::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {         ymwm::environment::commands::ChangeBorderColor::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {           ymwm::environment::commands::MoveWindowRight::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {           ymwm::environment::commands::FocusNextWindow::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {           ymwm::environment::commands::FocusPrevWindow::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    { ymwm::environment::commands::MoveFocusedWindowBackward::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {  ymwm::environment::commands::MoveFocusedWindowForward::type,
     Checks{ FocusedWindowMustBePresent::type }     },
    {   ymwm::environment::commands::IncreaseMainWindowRatio::type,
     Checks{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStackOrCentered::type } },
    {   ymwm::environment::commands::DecreaseMainWindowRatio::type,
     Checks{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStackOrCentered::type } },
    {    ymwm::environment::commands::SwapFocusedWindowOnTop::type,
     Checks{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStack::type,
     AtLeastTwoWindowsPresent::type }               },
    {           ymwm::environment::commands::MoveFocusOnGrid::type,
     Checks{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeGrid::type }                },
    {         ymwm::environment::commands::RotateStackLayout::type,
     Checks{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStack::type }          },
    {                 ymwm::environment::commands::AddWindow::type,
     Checks{ EventMustBeWindowAdded::type }         },
    {          ymwm::environment::commands::UpdateWindowName::type,
     Checks{ EventMustBeWindowNameUpdated::type }   },

    {               ymwm::environment::commands::FocusWindow::type,
     Checks{ EventMustBeMouseOverWindow::type }     },

    {              ymwm::environment::commands::RemoveWindow::type,
     Checks{ EventMustBeWindowRemoved::type }       }
  };
} // namespace ymwm::checks
