#pragma once

#include "AbstractKeyPress.h"
#include "AbstractMousePress.h"
#include "AbstractUnknownEvent.h"

#include <variant>

namespace ymwm::events {
  using Event =
      std::variant<AbstractKeyPress, AbstractMousePress, AbstractUnknownEvent>;
}
