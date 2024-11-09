#pragma once

#include "Command.h"
#include "Event.h"
#include "EventHash.h"
#include <unordered_map>

namespace ymwm {
	using EventsMap = std::unordered_map<events::Event, commands::Command, events::EventHasher>;
}
