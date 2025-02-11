#pragma once

#include "AbstractKeyPress.h"
#include "AbstractMousePress.h"
#include "AbstractUnknownEvent.h"
#include "MouseOverWindow.h"
#include "WindowAdded.h"
#include "WindowNameUpdated.h"
#include "WindowRemoved.h"

#include <variant>

namespace ymwm::events {
  using Event = std::variant<AbstractKeyPress,
                             AbstractMousePress,
                             AbstractUnknownEvent,
                             WindowAdded,
                             WindowNameUpdated,
                             MouseOverWindow,
                             WindowRemoved>;
}
