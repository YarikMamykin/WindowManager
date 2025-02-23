#include "events/AbstractMouseCode.h"

#include <X11/X.h>

namespace ymwm::events {
  AbstractMouseCode::Type AbstractMouseCode::Left{ Button1 };
  AbstractMouseCode::Type AbstractMouseCode::Right{ Button3 };
  AbstractMouseCode::Type AbstractMouseCode::Wheel{ Button2 };
  AbstractMouseCode::Type AbstractMouseCode::WheelUp{ Button4 };
  AbstractMouseCode::Type AbstractMouseCode::WheelDown{ Button5 };
} // namespace ymwm::events
