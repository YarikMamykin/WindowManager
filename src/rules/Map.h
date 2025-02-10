#pragma once

#include "environment/Command.h"
#include "rules/Rule.h"

#include <set>
#include <string_view>
#include <unordered_map>

namespace ymwm::rules {
  using Rules = std::set<std::string_view>;
  using Map = std::unordered_map<std::string_view, Rules>;

  static const inline Map rules_map{
    {               ymwm::environment::commands::CloseWindow::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {         ymwm::environment::commands::ChangeBorderColor::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {           ymwm::environment::commands::MoveWindowRight::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {           ymwm::environment::commands::FocusNextWindow::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {           ymwm::environment::commands::FocusPrevWindow::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    { ymwm::environment::commands::MoveFocusedWindowBackward::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {  ymwm::environment::commands::MoveFocusedWindowForward::type,
     Rules{ FocusedWindowMustBePresent::type }      },
    {   ymwm::environment::commands::IncreaseMainWindowRatio::type,
     Rules{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStackOrCentered::type } },
    {   ymwm::environment::commands::DecreaseMainWindowRatio::type,
     Rules{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStackOrCentered::type } },
    {    ymwm::environment::commands::SwapFocusedWindowOnTop::type,
     Rules{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStack::type,
     AtLeastTwoWindowsPresent::type }               },
    {           ymwm::environment::commands::MoveFocusOnGrid::type,
     Rules{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeGrid::type }                },
    {         ymwm::environment::commands::RotateStackLayout::type,
     Rules{ FocusedWindowMustBePresent::type,
     CurrentLayoutMustBeOneOfStack::type }          },
    {                 ymwm::environment::commands::AddWindow::type,
     Rules{ EventMustBeWindowAdded::type }          },
    {          ymwm::environment::commands::UpdateWindowName::type,
     Rules{ EventMustBeWindowNameUpdated::type }    },

    {               ymwm::environment::commands::FocusWindow::type,
     Rules{ EventMustBeMouseOverWindow::type }      },

    {              ymwm::environment::commands::RemoveWindow::type,
     Rules{ EventMustBeWindowRemoved::type }        }
  };
} // namespace ymwm::rules
