#include "events/AbstractKeyMask.h"

#include <X11/X.h>

namespace ymwm::events {
  AbstractKeyMask::Type AbstractKeyMask::Ctrl{ ControlMask };
  AbstractKeyMask::Type AbstractKeyMask::Alt{ Mod1Mask };
  AbstractKeyMask::Type AbstractKeyMask::Shift{ ShiftMask };
} // namespace ymwm::events
