#include "events/AbstractKeyCode.h"

#include <X11/keysym.h>

namespace ymwm::events {
  AbstractKeyCode::Type AbstractKeyCode::a{ XK_a };
  AbstractKeyCode::Type AbstractKeyCode::b{ XK_b };
  AbstractKeyCode::Type AbstractKeyCode::c{ XK_c };
  AbstractKeyCode::Type AbstractKeyCode::A{ XK_A };
  AbstractKeyCode::Type AbstractKeyCode::B{ XK_B };
  AbstractKeyCode::Type AbstractKeyCode::C{ XK_C };
} // namespace ymwm::events
