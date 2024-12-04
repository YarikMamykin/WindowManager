#include "environment/Command.h"
#include "environment/Environment.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractKeyPress.h"
#include "events/Map.h"

#include <variant>

int main(int, char**) {
  ymwm::events::Map events_map;
  events_map.emplace(
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::a,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl },
      ymwm::environment::commands::ExitRequested{});

  ymwm::environment::Environment env{ events_map };

  for (auto event = env.event(); not env.exit_requested();
       event = env.event()) {
    if (not events_map.contains(event)) {
      continue;
    }
    env.execute(events_map.at(event));
  }

  return 0;
}
